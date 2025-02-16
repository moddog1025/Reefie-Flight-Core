import sympy as sp

# Define symbolic variables
t = sp.Symbol('t')
m = sp.Symbol('m')  # Mass of the rocket

# Define piecewise force function (mock thrust curve segments based on user input)
F1 = sp.Symbol('F1')  # Thrust during first segment
F2 = sp.Symbol('F2')  # Thrust during second segment
F3 = sp.Symbol('F3')  # Thrust during third segment

# Define time breakpoints
t1, t2, t3, t4 = sp.symbols('t1 t2 t3 t4')  # Time intervals

# Acceleration functions in each time segment
a1 = (F1 / m) - 9.81  # Segment 1: 0 <= t < t1
a2 = (F2 / m) - 9.81  # Segment 2: t1 <= t < t2
a3 = (F3 / m) - 9.81  # Segment 3: t2 <= t < t3

# Velocity integration
v1 = sp.integrate(a1, t)  # Velocity function for segment 1
v2 = sp.integrate(a2, t) + v1.subs(t, t1)  # Velocity function for segment 2 with continuity
v3 = sp.integrate(a3, t) + v2.subs(t, t2)  # Velocity function for segment 3 with continuity

# Altitude integration
y1 = sp.integrate(v1, t)  # Altitude function for segment 1
y2 = sp.integrate(v2, t) + y1.subs(t, t1)  # Altitude function for segment 2 with continuity
y3 = sp.integrate(v3, t) + y2.subs(t, t2)  # Altitude function for segment 3 with continuity

# Display the altitude functions for each segment
y1, y2, y3
