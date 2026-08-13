## Circular Queue via Linked List Explanation

To implement a circular queue using a singly linked list, we connect the last node (tail) to the first node (head) so that tail->next = head.
During enqueue, the new node is added at the end and its next pointer is updated to head.
During dequeue, the head is moved to the next node and tail->next is updated to the new head.
Special care is taken when only one node is present, where both head and tail become NULL after deletion.
