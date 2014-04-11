#!/usr/bin/python

from physics import triad
from simple_rocket import rocket_obj
from sim_core import sim_exe_thread
from time import sleep, clock, time

# Make a descendent of of the core engine for our use.
class myCore(sim_exe_thread):

    # Override pre_quanta() to report each time quanta calculation.
    def pre_quanta(self):
        print "Quanta",self.sim_turn,":", self.sim_clock,"seconds.", len(self.items), "objects, wall time:", time() 

    # Override msg_handler() to report messages
    def msg_handler(self,sim_obj, msg):
        print (" "+str(self.sim_clock)+"sec "+sim_obj.name
            +" says ("+msg+") ").center(75,"=")
        sim_exe_thread.msg_handler(self,sim_obj,msg)
    
    # Override handle_bump() to report but not act
    def handle_bump(self,a,b):
        print (" BUMP "+a.name+":"+b.name+" ").center(75,"~")
        
    # Override overrun_handler to report but not act
    def overrun_handler(self, delay):
        print (str(self.sim_clock)+" secs: QUANTA OVERRUN "
            +str(delay * -1)+" sec, "
            +str(len(self.items))+" objs").center(75,"*")
        
# create a simulation core for our use.
core = myCore()

x = 0
x_lim = 75

def mk_rocket(core, x, x_lim):
    if x < x_lim:
        rocket = rocket_obj("Rocket_"+str(x).zfill(2))
        rocket.grav_vec(triad(0,0,-9.82))
        rocket.burn(triad(1,1,x*0.25),40)
    #    rocket.burn(triad(1,1,1.414),x*10)
        core.add_item(rocket)
        x += 1
    return x

# start the sim and record the time.    
core.enable()
start = time()

x = mk_rocket(core, x, x_lim)
x = mk_rocket(core, x, x_lim)

# report and check for end conditions once a second (more or less)
while core.items:
    print "Running second", time() - start
    sleep(0.9)
    x = mk_rocket(core, x, x_lim)

print;
print "Complete: sim_clock = ",core.sim_clock,", wall clock  = ", time()-start, ", quantas = ",core.sim_turn

exit()

