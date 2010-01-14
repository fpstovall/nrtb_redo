# Provides the main simulation loop as a separate thread.

import threading
from time import time, sleep
from sim_queues import work_queue

class sim_exe_thread(threading.Thread):

    def __init__(self):
        threading.Thread.__init__(self,name="sim_core")
        self.items = []
        self.messages = work_queue()
        self.sim_turn = 0
        self.sim_clock = 0.0
        self.process_lock = threading.RLock()
        self.quanta = 0.02
        self.enabled = threading.Event()
        self.single_step = False
        self.start()
        
    def add_item(self,item):
        self.process_lock.acquire()
        item.sim_core = self
        self.items.append(item)
        self.process_lock.release()

    def enable(self):
        self.enabled.set()

    def disable(self):
        self.enabled.clear()

    def set_single_step(self, s = False):
        self.process_lock.acquire()
        self.single_step = s
        self.process_lock.release()

    def pre_quanta(self):
        pass

    def post_quanta(self):
        pass

    def handle_bump(self,a,b):
        pass

    def overrun_handler(self, delay):
        pass        

    def push_message(self, sim_obj, msg):
        self.messages.push((sim_obj, msg))
        
    def msg_handler(self, sim_obj, msg):
        if msg == "remove":
            self.items.remove(sim_obj)
        else:
            print (" bad message "+msg+" ").str(75,"~")
            
    def run(self):
        next_tick = 0.0
        while True:
            # park if run is disabled.
            self.enabled.wait()
            # reset latch if single-stepping
            if self.single_step:
                self.enabled.clear()
                next_tick = 0.0            
            if not next_tick:
                next_tick = time() + self.quanta           
            # setup for processing
            self.process_lock.acquire()
            self.sim_turn += 1
            self.sim_clock = self.sim_turn * self.quanta
            self.pre_quanta()
            # Process all the objects in the object.
            # act on all pending messages
            while not self.messages.empty():
                item, msg = self.messages.pop()
                self.msg_handler(item, msg)
            # move all the objects
            for item in self.items:
                item.apply_quanta(self.quanta)
            # check for collisions
            for x in range(0,len(self.items)):
                Op, Or = self.items[x].bounding_sphere()
                for y in range(x+1,len(self.items)):
                    if self.items[y].bump(Op, Or):
                        # a collision has occurred.
                        self.handle_bump(self.items[x], self.items[y])
            # cleanup after processing
            self.post_quanta()
            self.process_lock.release()
            # sleep until time for next cycle.
            delay  = next_tick - time()
            if delay > 0:
                sleep(delay)
                next_tick += self.quanta
            else:
                self.overrun_handler(delay)
                next_tick = 0.0            
        

