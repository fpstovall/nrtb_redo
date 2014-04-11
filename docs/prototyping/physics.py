# Various core physics classes

class triad(object):

    def __init__(self, a=0.0, b=0.0, c=0.0):
        self.coords = (float(a),float(b),float(c))

    def set(self, a, b, c):
        x = float(a)
        y = float(b)
        z = float(c)
        self.coords = x, y, z
        return self

    def set_same(self, a):
        t = float(a)
        self.coords = t, t, t
        return self
        
    def add(self, other):
        t = triad()
        t.coords = ( self.coords[0] + other.coords[0],
                     self.coords[1] + other.coords[1],
                     self.coords[2] + other.coords[2] )
        return t
    
    def sub(self, other):
        t = triad()
        t.coords = ( self.coords[0] - other.coords[0],
                     self.coords[1] - other.coords[1],
                     self.coords[2] - other.coords[2] )
        return t
    
    def mul(self, other):
        t = triad()
        t.coords = ( self.coords[0] * other.coords[0],
                     self.coords[1] * other.coords[1],
                     self.coords[2] * other.coords[2] )
        return t
        
    def div(self, other):
        t = triad()
        t.coords = ( self.coords[0] / other.coords[0],
                     self.coords[1] / other.coords[1],
                     self.coords[2] / other.coords[2] )
        return t
        
    def pow(self, other):
        t = triad()
        t.coords = ( self.coords[0] ** other.coords[0],
                     self.coords[1] ** other.coords[1],
                     self.coords[2] ** other.coords[2] )
        return t
        
    def scale(self, val):
        t = triad()
        t.coords = ( self.coords[0] * val,
                     self.coords[1] * val,
                     self.coords[2] * val )
        return t
        
    def magnitude(self):
        t = self.mul(self)
        return sum(t.coords) ** 0.5
        
    def range(self, other):
        offset = self.sub(other)
        return offset.magnitude();

    def normalize(self):        
        return self.scale(1/self.magnitude())

    def dot_product(self, other):
        return sum(self.mul(other).coords)

    def cross_product(self, other):
        t = triad()
        x = ((self.coords[1] * other.coords[2])
                - (self.coords[2] * other.coords[1]))
        y = ((self.coords[2] * other.coords[0])
                - (self.coords[0] * other.coords[2]))
        z = ((self.coords[0] * other.coords[1])
                - (self.coords[1] * other.coords[0]))
        t.coords = x, y, z
        return t
 
    def format(self):
        output = "(" + str(self.coords[0])
        output += "," + str(self.coords[1])
        output += "," + str(self.coords[2]) + ")"
        return output

    def __str__(self):
        return self.format()
    


