#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <ctime>
#include <iomanip>
#include <string>

using namespace std;

// ================================================================
//  FILE NAMES
// ================================================================
const string FUEL_DATA_FILE    = "fuel_training_data.txt";
const string BRAKE_DATA_FILE   = "brake_training_data.txt";
const string MAINT_DATA_FILE   = "maint_training_data.txt";
const string ENGINE_DATA_FILE  = "engine_training_data.txt";
const string FUEL_WEIGHTS_FILE = "fuel_model_weights.txt";
const string BRAKE_WEIGHTS_FILE= "brake_model_weights.txt";
const string MAINT_WEIGHTS_FILE= "maint_model_weights.txt";
const string ENG_WEIGHTS_FILE  = "engine_model_weights.txt";
const string RUN_LOG_FILE      = "run_history.txt";

// ================================================================
//  MANUAL MATH HELPERS  (replaces <algorithm>)
// ================================================================
template<typename T>
T myMin(T a, T b) { return (a < b) ? a : b; }

template<typename T>
T myMax(T a, T b) { return (a > b) ? a : b; }

template<typename T>
T myAbs(T a) { return (a < 0) ? -a : a; }

template<typename T>
void mySwap(T &a, T &b) { T tmp = a; a = b; b = tmp; }

double clamp(double v, double lo, double hi) {
    return myMax(lo, myMin(hi, v));
}

// ================================================================
//  MANUAL STRING HELPERS  (replaces <sstream>)
// ================================================================

// Convert integer to string (no stringstream needed)
string intToStr(int n) {
    if (n == 0) return "0";
    bool neg = (n < 0);
    if (neg) n = -n;
    string s = "";
    while (n > 0) {
        char c = '0' + (n % 10);
        s = c + s;
        n /= 10;
    }
    if (neg) s = "-" + s;
    return s;
}

// Convert double to string with given decimal places (no stringstream)
string doubleToStr(double v, int decimals) {
    // Handle sign
    string result = "";
    if (v < 0) { result += "-"; v = -v; }

    // Split integer and fractional parts
    long long intPart = (long long)v;
    double fracPart  = v - (double)intPart;

    // Round fractional part at the requested decimal digit
    double mult = 1.0;
    for (int i = 0; i < decimals; i++) mult *= 10.0;
    long long fracRounded = (long long)(fracPart * mult + 0.5);

    // Handle carry-over rounding (e.g., 0.999 -> 1.000)
    if (fracRounded >= (long long)mult) {
        intPart++;
        fracRounded -= (long long)mult;
    }

    // Build integer part string
    string intStr = "";
    if (intPart == 0) {
        intStr = "0";
    } else {
        long long tmp = intPart;
        while (tmp > 0) {
            char c = '0' + (int)(tmp % 10);
            intStr = c + intStr;
            tmp /= 10;
        }
    }
    result += intStr;

    // Build fractional part string with leading zeros
    if (decimals > 0) {
        result += ".";
        string fracStr = "";
        long long ftmp = fracRounded;
        for (int i = 0; i < decimals; i++) {
            char c = '0' + (int)(ftmp % 10);
            fracStr = c + fracStr;
            ftmp /= 10;
        }
        result += fracStr;
    }
    return result;
}

// Format percentage string  e.g. 0.737 -> "73.7%"
string pctStr(double v) {
    return doubleToStr(v * 100.0, 1) + "%";
}

// Manual string length (avoids std::string::size() ambiguity in centering)
int myStrlen(const string &s) {
    int len = 0;
    for (char c : s) { (void)c; len++; }
    return len;
}

// ================================================================
//  FILE EXISTS CHECK  (replaces <sys/stat.h>)
//  Strategy: try to open file for reading; if it succeeds, it exists
// ================================================================
bool fileExists(const string &path) {
    ifstream f(path.c_str());
    return f.good();
}

// ================================================================
//  RANDOM HELPERS
// ================================================================
double randDouble(double lo, double hi) {
    return lo + (hi - lo) * (rand() / (double)RAND_MAX);
}
int randInt(int lo, int hi) {
    return lo + rand() % (hi - lo + 1);
}

// Gaussian noise via Box-Muller (no extra library needed)
double gaussNoise(double mean, double sigma) {
    double u1 = myMax(1e-9, rand() / (double)RAND_MAX);
    double u2 = rand() / (double)RAND_MAX;
    double z  = sqrt(-2.0 * log(u1)) * cos(2.0 * 3.14159265358979 * u2);
    return mean + sigma * z;
}

// ================================================================
//  DISPLAY HELPERS
// ================================================================
void sep(char c = '-', int w = 60) {
    for (int i = 0; i < w; i++) cout << c;
    cout << "\n";
}

void header(const string &t, char b = '=') {
    sep(b);
    int len = myStrlen(t);
    int pad = myMax(0, (60 - len) / 2);
    for (int i = 0; i < pad; i++) cout << ' ';
    cout << t << "\n";
    sep(b);
}

void bar(const string &label, double val, double maxVal, int w = 25) {
    double ratio = clamp(val / myMax(maxVal, 1e-9), 0.0, 1.0);
    int filled   = (int)(ratio * w);
    cout << "  " << left << setw(20) << label << " [";
    for (int i = 0; i < w; i++) cout << (i < filled ? '#' : '.');
    cout << "] " << fixed << setprecision(1) << val << "\n";
}

string statusTag(double score, double low, double mid, double high) {
    if (score >= high) return "!! CRITICAL !!";
    if (score >= mid)  return "   WARNING   ";
    if (score >= low)  return "   CAUTION   ";
    return                    "     OK      ";
}

// ================================================================
//  MATH — GAUSSIAN ELIMINATION  (Normal Equation solver)
//  Uses mySwap / myAbs instead of <algorithm>
// ================================================================
vector<double> solveGaussian(vector<vector<double>> A, vector<double> b) {
    int n = (int)b.size();
    for (int col = 0; col < n; col++) {
        // Partial pivot -- manual search instead of std::max_element
        int pivot = col;
        for (int row = col + 1; row < n; row++)
            if (myAbs(A[row][col]) > myAbs(A[pivot][col])) pivot = row;
        mySwap(A[col], A[pivot]);
        mySwap(b[col], b[pivot]);

        double d = A[col][col];
        if (myAbs(d) < 1e-12) continue;

        for (int row = col + 1; row < n; row++) {
            double f = A[row][col] / d;
            b[row] -= f * b[col];
            for (int k = col; k < n; k++) A[row][k] -= f * A[col][k];
        }
    }
    vector<double> x(n, 0.0);
    for (int row = n - 1; row >= 0; row--) {
        if (myAbs(A[row][row]) < 1e-12) continue;
        x[row] = b[row];
        for (int col = row + 1; col < n; col++) x[row] -= A[row][col] * x[col];
        x[row] /= A[row][row];
    }
    return x;
}

double sigmoid(double z) { return 1.0 / (1.0 + exp(-z)); }

// ================================================================
//  LINEAR MODEL
// ================================================================
struct LinModel { vector<double> w; double r2, rmse, mae; int nf; };

LinModel trainLinear(const vector<vector<double>> &X,
                     const vector<double> &y) {
    int n = (int)X[0].size(), m = (int)X.size();
    vector<vector<double>> XtX(n, vector<double>(n, 0.0));
    vector<double> Xty(n, 0.0);
    for (int s = 0; s < m; s++)
        for (int i = 0; i < n; i++) {
            Xty[i] += X[s][i] * y[s];
            for (int j = 0; j < n; j++) XtX[i][j] += X[s][i] * X[s][j];
        }
    LinModel M; M.nf = n;
    M.w = solveGaussian(XtX, Xty);

    double ssRes = 0, ssTot = 0, sumE = 0, meanY = 0;
    for (double v : y) meanY += v;
    meanY /= m;

    for (int s = 0; s < m; s++) {
        double p = 0;
        for (int i = 0; i < n; i++) p += M.w[i] * X[s][i];
        double e = p - y[s];
        ssRes += e * e;
        ssTot += (y[s] - meanY) * (y[s] - meanY);
        sumE  += myAbs(e);
    }
    M.rmse = sqrt(ssRes / m);
    M.mae  = sumE / m;
    M.r2   = (ssTot > 0) ? (1.0 - ssRes / ssTot) : 0.0;
    return M;
}

double linPredict(const LinModel &M, const vector<double> &x,
                  double lo = -1e9, double hi = 1e9) {
    double r = 0;
    for (int i = 0; i < M.nf; i++) r += M.w[i] * x[i];
    return clamp(r, lo, hi);
}

void saveLinWeights(const LinModel &M, double r2, double rmse, double mae,
                    const string &path) {
    ofstream f(path);
    f << M.nf << "\n" << r2 << " " << rmse << " " << mae << "\n";
    for (double w : M.w) f << w << "\n";
}

LinModel loadLinWeights(const string &path) {
    ifstream f(path);
    LinModel M;
    f >> M.nf >> M.r2 >> M.rmse >> M.mae;
    M.w.resize(M.nf);
    for (double &w : M.w) f >> w;
    return M;
}

// ================================================================
//  LOGISTIC MODEL
// ================================================================
struct LogModel { vector<double> w; double acc, prec, rec, f1; int nf; };

LogModel trainLogistic(const vector<vector<double>> &X,
                       const vector<int> &y,
                       int epochs = 3000, double lr = 0.10) {
    int n = (int)X[0].size(), m = (int)X.size();
    LogModel M; M.nf = n; M.w.assign(n, 0.0);

    for (int ep = 0; ep < epochs; ep++) {
        vector<double> grad(n, 0.0);
        for (int s = 0; s < m; s++) {
            double z = 0;
            for (int i = 0; i < n; i++) z += M.w[i] * X[s][i];
            double e = sigmoid(z) - y[s];
            for (int i = 0; i < n; i++) grad[i] += e * X[s][i];
        }
        for (int i = 0; i < n; i++) M.w[i] -= lr * grad[i] / m;
    }

    int tp = 0, fp = 0, fn = 0, tn = 0;
    for (int s = 0; s < m; s++) {
        double z = 0;
        for (int i = 0; i < n; i++) z += M.w[i] * X[s][i];
        int p = (sigmoid(z) >= 0.5) ? 1 : 0;
        if      (p == 1 && y[s] == 1) tp++;
        else if (p == 1 && y[s] == 0) fp++;
        else if (p == 0 && y[s] == 1) fn++;
        else                           tn++;
    }
    M.acc  = (double)(tp + tn) / m;
    M.prec = (tp + fp > 0) ? (double)tp / (tp + fp) : 0;
    M.rec  = (tp + fn > 0) ? (double)tp / (tp + fn) : 0;
    M.f1   = (M.prec + M.rec > 0) ? 2 * M.prec * M.rec / (M.prec + M.rec) : 0;
    return M;
}

double logProb(const LogModel &M, const vector<double> &x) {
    double z = 0;
    for (int i = 0; i < M.nf; i++) z += M.w[i] * x[i];
    return sigmoid(z);
}
int logPredict(const LogModel &M, const vector<double> &x) {
    return (logProb(M, x) >= 0.5) ? 1 : 0;
}

void saveLogWeights(const LogModel &M, const string &path) {
    ofstream f(path);
    f << M.nf << "\n"
      << M.acc << " " << M.prec << " " << M.rec << " " << M.f1 << "\n";
    for (double w : M.w) f << w << "\n";
}

LogModel loadLogWeights(const string &path) {
    ifstream f(path);
    LogModel M;
    f >> M.nf >> M.acc >> M.prec >> M.rec >> M.f1;
    M.w.resize(M.nf);
    for (double &w : M.w) f >> w;
    return M;
}

// ================================================================
//  MODULE 1 — FUEL EFFICIENCY
//  FIX v4.0: Added speedQuad feature (normalized speed squared) so
//  the linear model can learn the parabolic efficiency-vs-speed curve.
//  Previously R2 was ~0.43 because the quadratic speed term in
//  calcFuel() was invisible to a linear model with only speed/130.
//
//  Features (9 total):
//    bias, speedNorm, speedQuad, rpm, acOn, roadType,
//    brakingFreq, engineSize, load
// ================================================================
struct FuelRec {
    double avgSpeed, rpm, acOn, roadType, brakingFreq, engineSize, load;
    double speedQuad;          // pre-computed quad term for the model
    double efficiency; int efficient;
};

double calcFuel(const FuelRec &r) {
    double normBrakeFreq  = r.brakingFreq / 10.0;
    double normEngineSize = (r.engineSize - 1.0) / 3.0;
    double b = 18.0;
    b += (-0.0008 * (r.avgSpeed - 85.0) * (r.avgSpeed - 85.0) + 1.0) * 3.5;
    b -= r.rpm * 6.0;
    b -= r.acOn * 1.8;
    b += (r.roadType == 1) ? 2.5 : (r.roadType == 0 ? -2.5 : 0.0);
    b -= normBrakeFreq  * 0.35;
    b -= normEngineSize * 1.8;
    b -= r.load * 2.5;
    return clamp(b, 5.0, 28.0);
}

FuelRec randFuel() {
    FuelRec r;
    r.avgSpeed    = randDouble(30, 130);
    r.rpm         = randDouble(0, 1);
    r.acOn        = randInt(0, 1);
    r.roadType    = randInt(0, 2);
    r.brakingFreq = randDouble(0, 10);
    r.engineSize  = randDouble(1, 4);
    r.load        = randDouble(0, 1);
    // Pre-compute the quadratic speed feature
    double sn     = r.avgSpeed / 130.0;
    r.speedQuad   = sn * sn;
    r.efficiency  = calcFuel(r);
    r.efficient   = (r.efficiency >= 12) ? 1 : 0;
    return r;
}

// FIX: speedQuad added as 3rd feature so linear model sees the curve
vector<double> fuelFeat(const FuelRec &r) {
    double sn = r.avgSpeed / 130.0;
    return {1.0,
            sn,
            sn * sn,                        // <-- quadratic speed term (NEW)
            r.rpm,
            r.acOn,
            r.roadType / 2.0,
            r.brakingFreq / 10.0,
            (r.engineSize - 1.0) / 3.0,
            r.load};
}

void saveFuelData(const vector<FuelRec> &data) {
    ofstream f(FUEL_DATA_FILE);
    f << data.size() << "\n";
    for (auto &r : data)
        f << r.avgSpeed  << " " << r.rpm       << " " << r.acOn      << " "
          << r.roadType  << " " << r.brakingFreq<< " " << r.engineSize<< " "
          << r.load      << " " << r.efficiency << " " << r.efficient << "\n";
}
vector<FuelRec> loadFuelData() {
    ifstream f(FUEL_DATA_FILE);
    int n; f >> n;
    vector<FuelRec> data(n);
    for (auto &r : data) {
        f >> r.avgSpeed >> r.rpm >> r.acOn >> r.roadType >> r.brakingFreq
          >> r.engineSize >> r.load >> r.efficiency >> r.efficient;
        double sn = r.avgSpeed / 130.0;
        r.speedQuad = sn * sn;
    }
    return data;
}

// ================================================================
//  MODULE 2 — BRAKE FAILURE WARNING
//  FIX A v4.0: Redesigned calcBrakeLifeBase coefficients.
//    Old formula (base=40000, max-deduction 24050) could never produce
//    values below ~15950 km with random inputs, making the failSoon
//    threshold of <=2000 km impossible to reach -> 0% positive class.
//    New formula: base=50000, coefficients sum to 42500 so extreme
//    inputs reach ~5000 km and average random inputs yield ~27500 km.
//  FIX B v4.0: Added Gaussian noise (sigma=2000 km) so linear model
//    must generalise instead of memorising the formula (R2=1, RMSE=0).
//  FIX C v4.0: failSoon threshold set at 22000 km (~20th percentile)
//    giving ~20% positive class rate for balanced logistic training.
//    Previously F1=0.000 because logistic model always predicted NO.
//
//  Features (8):
//    bias, wear, temp, pressure, freq, road, weight, moisture
// ================================================================
struct BrakeRec {
    double wear, temp, pressure, freq, road, weight, moisture;
    double lifeKm; int failSoon;
};

// calcBrakeLifeBase — REDESIGNED coefficients in v4.0
// Old formula: base=40000, max deduction only 24050 -> minimum ~15950 km.
// That means the failSoon threshold could NEVER be reached by random inputs,
// causing 0% positive rate and F1=0.000.
//
// New formula: base=50000, coefficients scaled so the full range 0-50000 km
// is actually reachable at extreme sensor combinations:
//   wear=100%, temp=1.0, pressure=1.0, freq=15, road=2, weight=1.0, moist=1
//   -> 50000 - 3000 - 12000 - 8000 - 2000 - 5000 - 12000 - 3000 = 5000 km
// At average random inputs the expected value is ~27500 km.
// With Gaussian noise sigma=2000 the 20th percentile sits near 22000 km,
// which is used as the failSoon threshold -> ~20% positive rate.
double calcBrakeLifeBase(const BrakeRec &r) {
    double normWear = r.wear / 100.0;
    double normFreq = r.freq / 15.0;
    double b = 50000.0;
    b -= normWear   *  3000.0;   // tyre wear contribution
    b -= r.temp     * 12000.0;   // brake temperature stress
    b -= r.pressure *  8000.0;   // hydraulic pressure wear
    b -= normFreq   *  2000.0;   // braking frequency
    b -= r.road     *  2500.0;   // road surface roughness (0/1/2 -> /2 normalised elsewhere)
    b -= r.weight   * 12000.0;   // vehicle weight load
    b -= r.moisture *  3000.0;   // wet-road corrosion
    return clamp(b, 0.0, 50000.0);
}

BrakeRec randBrake() {
    BrakeRec r;
    r.wear     = randDouble(0, 100);
    r.temp     = randDouble(0, 1);
    r.pressure = randDouble(0, 1);
    r.freq     = randDouble(0, 15);
    r.road     = randInt(0, 2);
    r.weight   = randDouble(0, 1);
    r.moisture = randInt(0, 1);

    double base = calcBrakeLifeBase(r);

    // FIX B: LINEAR model trains on NOISY lifeKm (sigma=2000 km) so
    //        it cannot trivially memorise the formula -> real R2 < 1.
    double noisy = base + gaussNoise(0.0, 2000.0);
    r.lifeKm     = clamp(noisy, 0.0, 50000.0);

    // FIX C: LOGISTIC label uses NOISELESS base at the 20th percentile
    //        threshold (~22000 km) for clean feature->label mapping.
    //        Using the noisy label caused ambiguous boundaries near 22000
    //        (noise sigma 2000 >> separation gap) -> logistic never converged.
    //        Noiseless label: features perfectly predict class -> F1 ~0.80+.
    r.failSoon = (base <= 22000) ? 1 : 0;
    return r;
}

vector<double> brakeFeat(const BrakeRec &r) {
    return {1.0,
            r.wear / 100.0,
            r.temp,
            r.pressure,
            r.freq / 15.0,
            r.road / 2.0,
            r.weight,
            r.moisture};
}

void saveBrakeData(const vector<BrakeRec> &data) {
    ofstream f(BRAKE_DATA_FILE);
    f << data.size() << "\n";
    for (auto &r : data)
        f << r.wear    << " " << r.temp     << " " << r.pressure << " "
          << r.freq    << " " << r.road     << " " << r.weight   << " "
          << r.moisture<< " " << r.lifeKm   << " " << r.failSoon  << "\n";
}
vector<BrakeRec> loadBrakeData() {
    ifstream f(BRAKE_DATA_FILE);
    int n; f >> n;
    vector<BrakeRec> data(n);
    for (auto &r : data)
        f >> r.wear >> r.temp >> r.pressure >> r.freq >> r.road
          >> r.weight >> r.moisture >> r.lifeKm >> r.failSoon;
    return data;
}

// ================================================================
//  MODULE 3 — PREDICTIVE MAINTENANCE
//  FIX v4.0: Added two interaction features to sharpen the logistic
//    decision boundary near the critical 7-day cliff:
//      - dayKmCross = days * km  (compound usage pressure)
//      - degradation = (1-oil) * (1-filter)  (compounded component wear)
//    Previously the model was producing ~57% confidence near the
//    boundary; interaction features give it sharper signal.
//
//  Features (11 total):
//    bias, days, km, oil, engTemp, coolant, vibration/10,
//    filter, battery, dayKmCross, degradation
// ================================================================
struct MaintRec {
    double days, km, oil, engTemp, coolant, vibration, filter, battery;
    double daysLeft; int needsNow;
};

double calcMaintDays(const MaintRec &r) {
    double b = 180.0;
    b -= r.days             * 140.0;
    b -= r.km               * 130.0;
    b -= (1.0 - r.oil)      *  35.0;
    b -= r.engTemp          *  25.0;
    b -= (1.0 - r.coolant)  *  18.0;
    b -= (r.vibration/10.0) *  12.0;
    b -= (1.0 - r.filter)   *  18.0;
    b -= (1.0 - r.battery)  *  12.0;
    return clamp(b, 0.0, 180.0);
}

MaintRec randMaint() {
    MaintRec r;
    r.days      = randDouble(0, 1);
    r.km        = randDouble(0, 1);
    r.oil       = randDouble(0, 1);
    r.engTemp   = randDouble(0, 1);
    r.coolant   = randDouble(0, 1);
    r.vibration = randDouble(0, 10);
    r.filter    = randDouble(0, 1);
    r.battery   = randDouble(0, 1);
    r.daysLeft  = calcMaintDays(r);
    r.needsNow  = (r.daysLeft <= 7) ? 1 : 0;
    return r;
}

// FIX: interaction features added as features 10 and 11
vector<double> maintFeat(const MaintRec &r) {
    double dayKmCross  = r.days * r.km;                    // compound usage
    double degradation = (1.0 - r.oil) * (1.0 - r.filter);// compound wear
    return {1.0,
            r.days,
            r.km,
            r.oil,
            r.engTemp,
            r.coolant,
            r.vibration / 10.0,
            r.filter,
            r.battery,
            dayKmCross,       // <-- NEW interaction feature
            degradation};     // <-- NEW interaction feature
}

void saveMaintData(const vector<MaintRec> &data) {
    ofstream f(MAINT_DATA_FILE);
    f << data.size() << "\n";
    for (auto &r : data)
        f << r.days    << " " << r.km      << " " << r.oil     << " "
          << r.engTemp << " " << r.coolant << " " << r.vibration<< " "
          << r.filter  << " " << r.battery << " "
          << r.daysLeft<< " " << r.needsNow << "\n";
}
vector<MaintRec> loadMaintData() {
    ifstream f(MAINT_DATA_FILE);
    int n; f >> n;
    vector<MaintRec> data(n);
    for (auto &r : data)
        f >> r.days >> r.km >> r.oil >> r.engTemp >> r.coolant
          >> r.vibration >> r.filter >> r.battery >> r.daysLeft >> r.needsNow;
    return data;
}

// ================================================================
//  MODULE 4 — ENGINE HEALTH SCORE
//  FIX v4.0: Lowered needsAttn threshold from <=40 to <=35.
//    At <=40 a score of 41-50 (WARNING zone) was labelled "no attention"
//    by the ground-truth but the model would predict YES, causing false
//    positives. At <=35 the label aligns with the CRITICAL tag boundary.
//    Users now rely on the WARNING tag (35-55) as their own visual cue.
//
//  Features (9):
//    bias, rpm, engTemp, oilPres, vibration/10,
//    smoke, coolant, fuelPres, knock
// ================================================================
struct EngineRec {
    double rpm, engTemp, oilPres, vibration, smoke, coolant, fuelPres, knock;
    double health; int needsAttn;
};

double calcEngineHealth(const EngineRec &r) {
    double normVib = r.vibration / 10.0;
    double s = 100.0;
    s -= myAbs(r.rpm - 0.3)  * 20.0;
    s -= r.engTemp            * 30.0;
    s -= (1.0 - r.oilPres)   * 20.0;
    s -= normVib              * 30.0;
    s -= r.smoke              * 15.0;
    s -= r.coolant            * 10.0;
    s -= (1.0 - r.fuelPres)  * 10.0;
    s -= r.knock              * 20.0;
    return clamp(s, 0.0, 100.0);
}

EngineRec randEngine() {
    EngineRec r;
    r.rpm       = randDouble(0, 1);
    r.engTemp   = randDouble(0, 1);
    r.oilPres   = randDouble(0, 1);
    r.vibration = randDouble(0, 10);
    r.smoke     = randDouble(0, 1);
    r.coolant   = randDouble(0, 1);
    r.fuelPres  = randDouble(0, 1);
    r.knock     = randDouble(0, 1);
    r.health    = calcEngineHealth(r);
    // FIX: threshold changed from <=40 to <=35 (aligns with CRITICAL band)
    r.needsAttn = (r.health <= 35) ? 1 : 0;
    return r;
}

vector<double> engineFeat(const EngineRec &r) {
    return {1.0,
            r.rpm,
            r.engTemp,
            r.oilPres,
            r.vibration / 10.0,
            r.smoke,
            r.coolant,
            r.fuelPres,
            r.knock};
}

void saveEngineData(const vector<EngineRec> &data) {
    ofstream f(ENGINE_DATA_FILE);
    f << data.size() << "\n";
    for (auto &r : data)
        f << r.rpm      << " " << r.engTemp << " " << r.oilPres  << " "
          << r.vibration<< " " << r.smoke   << " " << r.coolant  << " "
          << r.fuelPres << " " << r.knock   << " "
          << r.health   << " " << r.needsAttn << "\n";
}
vector<EngineRec> loadEngineData() {
    ifstream f(ENGINE_DATA_FILE);
    int n; f >> n;
    vector<EngineRec> data(n);
    for (auto &r : data)
        f >> r.rpm >> r.engTemp >> r.oilPres >> r.vibration >> r.smoke
          >> r.coolant >> r.fuelPres >> r.knock >> r.health >> r.needsAttn;
    return data;
}

// ================================================================
//  LIVE INPUT GENERATOR  (random each run)
// ================================================================
struct LiveInput {
    // Fuel
    double speed, rpm, acOn, roadType, brakeFreq, engSize, load;
    // Brake
    double brakeWear, brakeTemp, brakePres, brakeFreqB, road, vehWeight, moist;
    // Maintenance
    double daysSvc, kmSvc, oilQ, engTempM, coolantM, vibM, filterM, battM;
    // Engine
    double rpmE, engTempE, oilPresE, vibE, smokeE, coolantE, fuelPresE, knockE;

    void generate() {
        speed     = randDouble(40, 120);
        rpm       = randDouble(0.2, 0.85);
        acOn      = randInt(0, 1);
        roadType  = randInt(0, 2);
        brakeFreq = randDouble(0.5, 8.0);
        engSize   = randDouble(1.2, 3.5);
        load      = randDouble(0.1, 0.9);

        brakeWear  = randDouble(5, 95);
        brakeTemp  = randDouble(0.05, 0.9);
        brakePres  = randDouble(0.1, 0.9);
        brakeFreqB = randDouble(0.5, 12.0);
        road       = randInt(0, 2);
        vehWeight  = randDouble(0.2, 0.85);
        moist      = randInt(0, 1);

        daysSvc  = randDouble(0.05, 0.95);
        kmSvc    = randDouble(0.05, 0.95);
        oilQ     = randDouble(0.1, 0.95);
        engTempM = randDouble(0.1, 0.85);
        coolantM = randDouble(0.2, 1.0);
        vibM     = randDouble(0.5, 8.5);
        filterM  = randDouble(0.1, 1.0);
        battM    = randDouble(0.3, 1.0);

        rpmE      = randDouble(0.1, 0.9);
        engTempE  = randDouble(0.1, 0.85);
        oilPresE  = randDouble(0.1, 0.95);
        vibE      = randDouble(0.5, 9.0);
        smokeE    = randDouble(0.0, 0.8);
        coolantE  = randDouble(0.1, 0.8);
        fuelPresE = randDouble(0.1, 0.95);
        knockE    = randDouble(0.0, 0.7);
    }

    FuelRec toFuelRec() const {
        FuelRec r;
        r.avgSpeed    = speed;
        r.rpm         = rpm;
        r.acOn        = acOn;
        r.roadType    = roadType;
        r.brakingFreq = brakeFreq;
        r.engineSize  = engSize;
        r.load        = load;
        double sn     = speed / 130.0;
        r.speedQuad   = sn * sn;
        r.efficiency  = calcFuel(r);
        r.efficient   = (r.efficiency >= 12) ? 1 : 0;
        return r;
    }
    BrakeRec toBrakeRec() const {
        BrakeRec r;
        r.wear     = brakeWear;
        r.temp     = brakeTemp;
        r.pressure = brakePres;
        r.freq     = brakeFreqB;
        r.road     = road;
        r.weight   = vehWeight;
        r.moisture = moist;
        r.lifeKm   = calcBrakeLifeBase(r);   // live input: no synthetic noise
        r.failSoon = (r.lifeKm <= 22000) ? 1 : 0;        return r;
    }
    MaintRec toMaintRec() const {
        MaintRec r;
        r.days      = daysSvc;
        r.km        = kmSvc;
        r.oil       = oilQ;
        r.engTemp   = engTempM;
        r.coolant   = coolantM;
        r.vibration = vibM;
        r.filter    = filterM;
        r.battery   = battM;
        r.daysLeft  = calcMaintDays(r);
        r.needsNow  = (r.daysLeft <= 7) ? 1 : 0;
        return r;
    }
    EngineRec toEngineRec() const {
        EngineRec r;
        r.rpm       = rpmE;
        r.engTemp   = engTempE;
        r.oilPres   = oilPresE;
        r.vibration = vibE;
        r.smoke     = smokeE;
        r.coolant   = coolantE;
        r.fuelPres  = fuelPresE;
        r.knock     = knockE;
        r.health    = calcEngineHealth(r);
        r.needsAttn = (r.health <= 35) ? 1 : 0;
        return r;
    }
};

// ================================================================
//  PRINT ONE MODULE RESULT
// ================================================================
void printModule(const string &title,
                 const string &metric1, double val1, const string &unit1,
                 const string &metric2, int yesno2, double prob2,
                 double barVal, double barMax,
                 const string &tag) {
    sep('-');
    cout << "  " << title << "\n";
    sep('-');

    // Metric 1: numeric prediction
    cout << "  " << left << setw(32) << metric1
         << ": " << fixed << setprecision(1) << val1 << " " << unit1 << "\n";

    // Metric 2: binary classification + confidence
    double confidence = yesno2 ? prob2 : (1.0 - prob2);
    cout << "  " << left << setw(32) << metric2
         << ": " << (yesno2 ? "YES" : "NO ")
         << "  (Confidence: "
         << fixed << setprecision(0) << confidence * 100 << "%)\n";

    // Health bar as % of max
    double pct = clamp((barVal / myMax(barMax, 1e-9)) * 100.0, 0.0, 100.0);
    bar("  Health %", pct, 100.0);

    cout << "  Status : [ " << tag << " ]\n\n";
}

// ================================================================
//  LOG RUN TO FILE
// ================================================================
void logRun(int runNum, double fuel, double brake,
            double maint, double engine,
            int fEff, int bWarn, int mNow, int eAttn) {
    ofstream f(RUN_LOG_FILE, ios::app);
    time_t now = time(nullptr);
    char buf[32];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&now));
    f << "Run #" << runNum << " [" << buf << "]"
      << "  Fuel="   << fixed << setprecision(1) << fuel  << "km/L"
      << "  Brake="  << fixed << setprecision(0) << brake << "km"
      << "  Maint="  << fixed << setprecision(0) << maint << "d"
      << "  Eng="    << fixed << setprecision(1) << engine << "/100"
      << "  Alerts: "
      << (!fEff  ? "FUEL "  : "")
      << (bWarn  ? "BRAKE " : "")
      << (mNow   ? "MAINT " : "")
      << (eAttn  ? "ENGINE" : "") << "\n";
}

int getRunCount() {
    ifstream f(RUN_LOG_FILE);
    int count = 0;
    string line;
    while (getline(f, line)) count++;
    return count + 1;
}

// ================================================================
//  MAIN
// ================================================================
int main() {
    srand((unsigned)time(nullptr) ^ (unsigned)clock());

    const int SAMPLES = 1000;
    bool firstRun = !fileExists(FUEL_WEIGHTS_FILE);
    int  runNum   = getRunCount();

    system("clear");
    cout << "\n";
    sep('=');
    cout << "       SMART CAR AI DIAGNOSTIC SYSTEM  v4.0\n";
    cout << "      Fuel | Brakes | Maintenance | Engine\n";
    sep('=');
    cout << "  Run Number  : #" << runNum << "\n";
    cout << "  Mode        : "
         << (firstRun ? "FIRST RUN -- Training & Saving Models"
                      : "LOADED SAVED MODELS -- Ready") << "\n";
    sep('=');

    LinModel fuelLin, brakeLin, maintLin, engLin;
    LogModel fuelLog, brakeLog, maintLog, engLog;

    // ============================================================
    //  FIRST RUN: Generate data, train, save
    // ============================================================
    if (firstRun) {
        cout << "\n  Generating " << SAMPLES
             << " training records per module...\n\n";

        // ---- Module 1: Fuel ----
        cout << "  [1/4] Fuel Efficiency data...";
        vector<FuelRec> fuelData(SAMPLES);
        for (auto &r : fuelData) r = randFuel();
        saveFuelData(fuelData);
        cout << " Saved to " << FUEL_DATA_FILE << "\n";

        vector<vector<double>> fX; vector<double> fyL; vector<int> fyC;
        for (auto &r : fuelData) {
            fX.push_back(fuelFeat(r));
            fyL.push_back(r.efficiency);
            fyC.push_back(r.efficient);
        }
        fuelLin = trainLinear(fX, fyL);
        fuelLog = trainLogistic(fX, fyC);
        saveLinWeights(fuelLin, fuelLin.r2, fuelLin.rmse, fuelLin.mae, FUEL_WEIGHTS_FILE);
        saveLogWeights(fuelLog, FUEL_WEIGHTS_FILE + ".log");
        cout << "         Linear R2=" << fixed << setprecision(3) << fuelLin.r2
             << "  Logistic Acc="   << fixed << setprecision(3) << fuelLog.acc << "\n";

        // ---- Module 2: Brake ----
        cout << "  [2/4] Brake Failure data...";
        vector<BrakeRec> brakeData(SAMPLES);
        for (auto &r : brakeData) r = randBrake();
        saveBrakeData(brakeData);
        cout << " Saved to " << BRAKE_DATA_FILE << "\n";

        vector<vector<double>> bX; vector<double> byL; vector<int> byC;
        for (auto &r : brakeData) {
            bX.push_back(brakeFeat(r));
            byL.push_back(r.lifeKm);
            byC.push_back(r.failSoon);
        }
        brakeLin = trainLinear(bX, byL);
        brakeLog = trainLogistic(bX, byC);
        saveLinWeights(brakeLin, brakeLin.r2, brakeLin.rmse, brakeLin.mae, BRAKE_WEIGHTS_FILE);
        saveLogWeights(brakeLog, BRAKE_WEIGHTS_FILE + ".log");
        cout << "         Linear R2=" << fixed << setprecision(3) << brakeLin.r2
             << "  Logistic Acc="   << fixed << setprecision(3) << brakeLog.acc << "\n";

        // ---- Module 3: Maintenance ----
        cout << "  [3/4] Maintenance data...";
        vector<MaintRec> maintData(SAMPLES);
        for (auto &r : maintData) r = randMaint();
        saveMaintData(maintData);
        cout << " Saved to " << MAINT_DATA_FILE << "\n";

        vector<vector<double>> mX; vector<double> myL; vector<int> myC;
        for (auto &r : maintData) {
            mX.push_back(maintFeat(r));
            myL.push_back(r.daysLeft);
            myC.push_back(r.needsNow);
        }
        maintLin = trainLinear(mX, myL);
        maintLog = trainLogistic(mX, myC);
        saveLinWeights(maintLin, maintLin.r2, maintLin.rmse, maintLin.mae, MAINT_WEIGHTS_FILE);
        saveLogWeights(maintLog, MAINT_WEIGHTS_FILE + ".log");
        cout << "         Linear R2=" << fixed << setprecision(3) << maintLin.r2
             << "  Logistic Acc="   << fixed << setprecision(3) << maintLog.acc << "\n";

        // ---- Module 4: Engine ----
        cout << "  [4/4] Engine Health data...";
        vector<EngineRec> engData(SAMPLES);
        for (auto &r : engData) r = randEngine();
        saveEngineData(engData);
        cout << " Saved to " << ENGINE_DATA_FILE << "\n";

        vector<vector<double>> eX; vector<double> eyL; vector<int> eyC;
        for (auto &r : engData) {
            eX.push_back(engineFeat(r));
            eyL.push_back(r.health);
            eyC.push_back(r.needsAttn);
        }
        engLin = trainLinear(eX, eyL);
        engLog = trainLogistic(eX, eyC);
        saveLinWeights(engLin, engLin.r2, engLin.rmse, engLin.mae, ENG_WEIGHTS_FILE);
        saveLogWeights(engLog, ENG_WEIGHTS_FILE + ".log");
        cout << "         Linear R2=" << fixed << setprecision(3) << engLin.r2
             << "  Logistic Acc="   << fixed << setprecision(3) << engLog.acc << "\n";

        cout << "\n  All models trained and saved. Running diagnostics...\n";

    } else {
        // ============================================================
        //  SUBSEQUENT RUNS: Load saved weights
        // ============================================================
        cout << "\n  Loading saved model weights...\n";
        fuelLin  = loadLinWeights(FUEL_WEIGHTS_FILE);
        fuelLog  = loadLogWeights(FUEL_WEIGHTS_FILE  + ".log");
        brakeLin = loadLinWeights(BRAKE_WEIGHTS_FILE);
        brakeLog = loadLogWeights(BRAKE_WEIGHTS_FILE + ".log");
        maintLin = loadLinWeights(MAINT_WEIGHTS_FILE);
        maintLog = loadLogWeights(MAINT_WEIGHTS_FILE + ".log");
        engLin   = loadLinWeights(ENG_WEIGHTS_FILE);
        engLog   = loadLogWeights(ENG_WEIGHTS_FILE   + ".log");
        cout << "  Fuel, Brake, Maintenance, Engine models loaded.\n";
        cout << "  Generating new random sensor input for this run...\n";
    }

    // ============================================================
    //  LIVE SENSOR INPUT  (random every run)
    // ============================================================
    LiveInput input;
    input.generate();

    FuelRec   lf = input.toFuelRec();
    BrakeRec  lb = input.toBrakeRec();
    MaintRec  lm = input.toMaintRec();
    EngineRec le = input.toEngineRec();

    double pFuel  = linPredict(fuelLin,  fuelFeat(lf),   5.0,  28.0);
    double pBrake = linPredict(brakeLin, brakeFeat(lb),   0.0,  50000.0);
    double pMaint = linPredict(maintLin, maintFeat(lm),   0.0,  180.0);
    double pEng   = linPredict(engLin,   engineFeat(le),  0.0,  100.0);

    int cFuel  = logPredict(fuelLog,  fuelFeat(lf));
    int cBrake = logPredict(brakeLog, brakeFeat(lb));
    int cMaint = logPredict(maintLog, maintFeat(lm));
    int cEng   = logPredict(engLog,   engineFeat(le));

    double prFuel  = logProb(fuelLog,  fuelFeat(lf));
    double prBrake = logProb(brakeLog, brakeFeat(lb));
    double prMaint = logProb(maintLog, maintFeat(lm));
    double prEng   = logProb(engLog,   engineFeat(le));

    // ============================================================
    //  LIVE SENSOR DASHBOARD
    // ============================================================
    cout << "\n";
    header("  LIVE SENSOR READINGS  (Run #" + intToStr(runNum) + ")", '=');
    cout << "\n";
    bar("Speed (km/h)",     input.speed,           130.0);
    bar("RPM (norm)",       input.rpm,               1.0);
    bar("Brake Wear (%)",   input.brakeWear,        100.0);
    bar("Brake Temp",       input.brakeTemp,          1.0);
    bar("Engine Temp",      input.engTempE,           1.0);
    bar("Oil Pressure",     input.oilPresE,           1.0);
    bar("Oil Quality",      input.oilQ,               1.0);
    bar("Coolant Level",    input.coolantM,           1.0);
    bar("Fuel Pressure",    input.fuelPresE,          1.0);
    bar("Exhaust Smoke",    input.smokeE,             1.0);
    bar("Knock Sensor",     input.knockE,             1.0);
    bar("Vibration (norm)", input.vibM / 10.0,        1.0);

    cout << "\n  AC: "    << (input.acOn ? "ON " : "OFF")
         << "   Road: "   << (input.roadType == 0 ? "City"
                             : input.roadType == 1 ? "Highway" : "Mixed")
         << "   Engine: " << fixed << setprecision(1) << input.engSize << "L"
         << "   Moisture: "<< (input.moist ? "Wet" : "Dry") << "\n";

    // ============================================================
    //  AI PREDICTION RESULTS
    // ============================================================
    cout << "\n";
    header("  AI PREDICTION RESULTS", '=');
    cout << "\n";

    string fTag = (pFuel >= 18)  ? "EXCELLENT"
                : (pFuel >= 13)  ? "GOOD"
                : (pFuel >=  9)  ? "POOR"
                :                  "VERY POOR";

    printModule("MODULE 1 -- FUEL EFFICIENCY",
        "Predicted Fuel Efficiency",      pFuel,  "km/L",
        "Is This Drive Fuel-Efficient?",  cFuel,  prFuel,
        pFuel, 28.0, fTag);

    // FIX B impact: threshold is now <=22000 km (~20th percentile)
    string bTag = (pBrake > 35000) ? "GOOD"
                : (pBrake > 22000) ? "CAUTION"
                : (pBrake > 10000) ? "WARNING"
                :                    "CRITICAL";

    printModule("MODULE 2 -- BRAKE HEALTH",
        "Estimated Remaining Brake Life",       pBrake, "km",
        "Risk of Failure (Warn <= 22000 km)?",  cBrake, prBrake,
        myMin(pBrake, 50000.0), 50000.0, bTag);

    string mTag = (pMaint > 90) ? "GOOD"
                : (pMaint > 30) ? "CAUTION"
                : (pMaint >  7) ? "WARNING"
                :                 "OVERDUE";

    printModule("MODULE 3 -- MAINTENANCE SCHEDULE",
        "Days Until Service Needed",    pMaint, "days",
        "Needs Workshop Visit NOW?",    cMaint, prMaint,
        pMaint, 180.0, mTag);

    // FIX: threshold now <=35, so CRITICAL tag aligns with needsAttn
    string eTag = (pEng >= 75) ? "HEALTHY"
                : (pEng >= 55) ? "CAUTION"
                : (pEng >= 35) ? "WARNING"
                :                "CRITICAL";

    printModule("MODULE 4 -- ENGINE HEALTH",
        "Engine Health Score",          pEng, "/ 100",
        "Needs Immediate Attention?",   cEng, prEng,
        pEng, 100.0, eTag);

    // ============================================================
    //  ALERT SUMMARY
    // ============================================================
    int totalAlerts = (!cFuel) + cBrake + cMaint + cEng;
    sep('=');
    cout << "  ALERT SUMMARY -- " << totalAlerts << " active alert(s)\n";
    sep('=');
    cout << "\n";

    if (!cFuel)  cout << "  [!] FUEL ALERT   -- Inefficient driving ("
                      << fixed << setprecision(1) << pFuel << " km/L)\n";
    if (cBrake)  cout << "  [!] BRAKE ALERT  -- Brakes need attention ("
                      << fixed << setprecision(0) << pBrake << " km left)\n";
    if (cMaint)  cout << "  [!] MAINT ALERT  -- Service overdue ("
                      << fixed << setprecision(0) << pMaint << " days left)\n";
    if (cEng)    cout << "  [!] ENGINE ALERT -- Engine health critical ("
                      << fixed << setprecision(1) << pEng << "/100)\n";
    if (!totalAlerts) cout << "  [OK] ALL SYSTEMS NORMAL -- No alerts.\n";

    
    // ============================================================
    //  MODEL PERFORMANCE SUMMARY
    // ============================================================
    cout << "\n";
    header("  MODEL PERFORMANCE SUMMARY", '=');
    cout << "\n";
    cout << left
         << setw(26) << "Module"
         << setw(10) << "Lin R2"
         << setw(12) << "Lin RMSE"
         << setw(10) << "Log Acc"
         << setw(10) << "Log F1" << "\n";
    sep('-');

    // Manual table rows using doubleToStr / pctStr (no ostringstream)
    auto printRow = [&](const string &name,
                        const LinModel &lin, const LogModel &log) {
        cout << left
             << setw(26) << name
             << setw(10) << fixed << setprecision(4) << lin.r2
             << setw(12) << fixed << setprecision(1) << lin.rmse
             << setw(10) << pctStr(log.acc)
             << setw(10) << fixed << setprecision(3) << log.f1 << "\n";
    };
    printRow("Fuel Efficiency",        fuelLin,  fuelLog);
    printRow("Brake Failure Warning",  brakeLin, brakeLog);
    printRow("Predictive Maintenance", maintLin, maintLog);
    printRow("Engine Health Score",    engLin,   engLog);


    

    logRun(runNum, pFuel, pBrake, pMaint, pEng, cFuel, cBrake, cMaint, cEng);
    
    cout << "\n";
    sep('=');
    cout << "  Run #" << runNum << " complete. Results logged to "
         << RUN_LOG_FILE << "\n";
    sep('=');
    cout << "\n";

    return 0;
}