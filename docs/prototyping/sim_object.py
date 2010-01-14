# abstract abs_sim_obj, parent of all objects in a simulation. 

from physics import triad
import threading

class abs_sim_obj(object):

    def __init__(self, name):
        self.name = name
        self.position = triad() # start at 0,0,0
        self.velocity = triad() # start at 0,0,0
        self.pending_imp = triad()
        self.pending_rot_imp = triad()
        self.mass = 1.0 # kilograms
        self.gravity = triad()
        self.processing = threading.RLock()
        self.sim_core = 0  # must be a sim_exe_thread object before use.

    def impulse(self, imp):
        self.processing.acquire()
        self.pending_imp = self.pending_imp.add(imp)
        self.processing.release()
    
    def total_mass(self):
        return self.mass
        
    def grav_vec(self, vector):
        self.processing.acquire()
        self.gravity  = vector
        self.processing.release()

    def bounding_sphere(self):
        return self.position, 1.0

    def bump(self, Op, Or):
        Sp, Sr = self.bounding_sphere()
        t = Sp.sub(Op)
        t = t.mul(t)
        D2 = sum(t.coords)
        R2 = Sr + Or
        R2 *= R2
        return (R2 >= D2)
              
    def pre_apply(self, time_span):
        pass

    def post_apply(self, time_span):
        pass

    def apply_quanta(self, time_span):
        self.processing.acquire()
        # Working vars
        start_mass = self.total_mass()
        # call pre actions
        self.pre_apply(time_span)
        # working mass for movement.
        avg_mass = (self.total_mass() + start_mass)/2
        # apply the results for impulse
        new_v_component = self.pending_imp.scale(1/avg_mass).scale(time_span)
        # apply gravity
        new_v_component = new_v_component.add(self.gravity.scale(time_span))
        # apply velocity to position
        self.position = self.position.add(self.velocity.scale(time_span))
        self.position = self.position.add(
                            new_v_component.scale(0.5).scale(time_span))
        # Update velocity for the next quanta
        self.velocity = self.velocity.add(new_v_component)
        # clear out the pendings.
        self.pending_imp.set_same(0)
        # call post actions
        self.post_apply(time_span)
        self.processing.release()


