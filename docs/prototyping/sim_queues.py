# Implements a general purpose, thread-safe queue.

import threading

class work_queue(object):

    def __init__(self):
        self.my_queue = [] 
        self.guard = threading.Condition()

    # places something in the queue
    def push(self, item):
        self.guard.acquire()
        self.my_queue.append(item)
        self.guard.notify()
        self.guard.release()

    # removes one item from the queue
    def pop(self):
        self.guard.acquire()
        returnme = self.my_queue.pop()
        self.guard.release()
        return returnme

    # like pop(), but blocks if the queue is empty
    def park_n_pop(self):
        self.guard.acquire()
        while not self.my_queue:
            self.wait()
        returnme = self.my_queue.pop()
        self.guard.release()
        return returnme        

    # returns the number of items in the queue
    def len(self):
        return len(self.my_queue)

    # returns True if the queue is empty, False otherwise.
    def empty(self):
        return len(self.my_queue) == 0
