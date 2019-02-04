# Dominant-cars-angle-edge-detector-in-traffic
The aim is to find angle between cars' front edge and frame horizont for front view normalization.
Just launch command line "traffic_angle.exe your_traffic_video.mp4"

This program accumulates traffic info and find statistically dominating
angle of front cars' edges. It can be used for normalizing front view.

Restriction:
1) Accuracy is not high, but may be easily improved if nessessary;
2) Only stright line traffic can be measured
3) Angles from -45 to +45 degrees.
4) Some time (or cars passed) should be waited to get statistically approved result

Further improvement can be done: this set of gauge-lines can be presented as Bresenham's line algorithm,
thus possible getting equal angle intervals, which leads to higher accuracy. In my case I did it only for
demo and so used simpler algorithm.
