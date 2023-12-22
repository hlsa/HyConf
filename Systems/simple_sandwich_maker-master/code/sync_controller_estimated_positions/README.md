This is a implementation of the controller for the robotic arm.

It uses explicit synchronisation to delimit the simulation cycle (instead of time),
and estimates the position of the arm instead o using a sensor. This introduces
errors that increase with time, and invariably lead to failure.
