# Computer Vision
 An OpenFrameworks project.  Uses the webcam to detect two colours and determine if the colours are touching.

Controls:
Select Colour 1 - Right Mouse Button
Select Colour 2 - Left Mouse Button
	
I created a colour tracker that would track two separate colours.  The user is allowed to select the colours by either right or middle mouse button click.  Using a GUI, the user is able to vary the tolerance, minimum radius, and maximum radius of the colour they are tracking. The background colour will change to blue if the colours are near touching.  Text will display below the video feed if a colour was lost and a red dot beside the colour indicators (large circles) will appear if a colour is being tracked.  Contour blobs are drawn around the colour that is being tracked