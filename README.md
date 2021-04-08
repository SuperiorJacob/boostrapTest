## Table of contents
* [General info](#general-info)
* [Technologies](#technologies)
* [Graphics Usage](#graphics-usage)

## General info
Author: Jacob Cooper (s200503@students.aie.edu.au)

School: Academy of Interactive Entertainment (AIE)

This project includes two bootstrap assessments:
* Physics for Games project of pachinko (``/PhysicsProject``)
* 3D OpenGL Application (``/GraphicsProject``)
	
## Technologies
Project is created with:
* Bootstrap
* ImGui
* C++
* Visual Studio
	
## Graphics Usage
#### Application
Graphics Project usage, to compile open ``../Bootstrap.sln`` and select your desired compile type ``Release/Debug`` and ``x32/x64 bit``, right click the project and click rebuild. It should compile to bin or ``temp/desired output``. 

To run the program just double click the provided release .exe, and to run it in debug open the .sln and click run.
#### Operation
Operation of the project in runtime can be found here.

There are four cameras (which can be switched between by pressing tab):
* Flyby (controlled using WASD for 4 dimensional movement, and the mouse to turn the camera.
* 3 Stationary Cameras (look directly at a position can not be controlled).

ImGui editor to manipulate the scene:

| Category | Name | Description |
| --- | --- | --- |
| Settings |
| Settings | Sunlight Direction | Using a Vector3 slider, manipulate the direction the light faces.
| Settings | Sunlight Color | Using a Vector3 slider, manipulate the suns color.
| Settings | Enable Distort Post-processing | Enable/Disable to use distorted post processing.
| Objects |
| Objects/Game Objects | Every visible mesh object in the scene. | Drop the category, you can manipulate the position, rotation and size of the object.
| Objects/Lights | All lights in the scene | Can manipulate the position, color and the intensity (effect radius) of the lights.