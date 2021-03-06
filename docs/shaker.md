# Shaker

Shake your Rack. This module can modulate the cable tension, opacity, the
zoom factor and the scroll position of your rack.

![alt text](shaker.png "Shaker")

### Tension
This input has a range from 0 V to 10 V and changes the cable tension
from 0 to 1.

### Opacity
The opacity input has the same range from 0 V to 10 V and changes the cable
opacity from 0 % to 100%.

### Zoom
The zoom input has a range from -10 V to +10 V. This is scaled down
to the range -2 to +2. The formula used by Rack to calculate the
zoom factor is 2^n. So for an input voltage of 0 V, the zoom factor
is 1, or 100%. For -10 V it is 25% and for +10 V it is 400%.

You can start and stop the effect with the on/off button. If a cable is connected
to the zoom input, you can also stop the effect by manually zooming,
for example by holding down the ctrl-key and moving the mouse wheel.

If your rack is zooming or moving like crazy and you can't stop it, go to the
menu `Engine->Sample Rate->Pause`:
	
![alt text](engine-pause.png "Engine pause")

This sets the on/off button to the off position as well.
