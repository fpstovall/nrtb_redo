# simple rocket class example

from sim_object import abs_sim_obj
from physics import triad

class rocket_obj(abs_sim_obj):

    def __init__(self, name):
        abs_sim_obj.__init__(self, name)
        self.mass = 10
        self.fuel_mass = 20.0 # kilograms
        self.specific_impulse = 100.0 # newtons/kg
        self.burn_rate = 0.0
        self.burn_vector = triad()
        self.active = True

    # impulse() is overridden to provide an output line.
    def impulse(self, imp):
        print self.name, "IMPULSE:",imp.magnitude(),"newtons/second."
        abs_sim_obj.impulse(self,imp)
    
    # total_mass() is overridden to include the fuel mass.
    def total_mass(self):
        return self.mass + self.fuel_mass
        
    # New method providing the thruster control interface.
    def burn(self, vec, rate):
        if self.fuel_mass > 0.0:
            self.burn_rate = float(rate)
            self.burn_vector = vec.normalize().scale(
                            self.burn_rate * self.specific_impulse) 

    # pre_apply() is overridden to implement the thruster.
    def pre_apply(self, time_span):
        if self.burn_rate:
            print self.name,"Thruster Active"
            fuel_to_burn = self.burn_rate * time_span
            if self.fuel_mass < fuel_to_burn:
                # drain the tank
                self.impulse(
                    self.burn_vector.normalize().scale( 
                        (self.fuel_mass 
                            * self.specific_impulse) / time_span ) )
                self.fuel_mass = 0.0
            else:
                # enough fuel
                self.impulse(self.burn_vector)
                self.fuel_mass -= fuel_to_burn
            if self.fuel_mass <= 0.0:
                self.burn_rate = 0.0
                print self.name,"BURNOUT"

    # post_apply() is overridden for reporting and impact handling
    def post_apply(self, time_span):
        if self.active:
            if self.position.coords[2] <= 0.0:
                print self.name, "IMPACT v =", self.velocity.magnitude(), "range =", self.position.magnitude()
                self.sim_core.push_message(self,"remove")
            else:
                print self.name, "v =", self.velocity.magnitude(), "alt =", self.position.coords[2], "range =", self.position.magnitude()
        

