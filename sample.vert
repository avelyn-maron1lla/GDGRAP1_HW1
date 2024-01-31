#version 330 core

//gets the data at Attrib Index 0
//Converts it and stores it into a Vec3
layout(location = 0) in vec3 aPos;

//declare a variable to hold data and then PASS IT

//transform matrix babyyyy
uniform mat4 transform;

//Projection Matrix
uniform mat4 projection;

void main()
{
	//gl_Position is predefined
	//denotes final position of the vertex/point
	gl_Position = projection * transform * vec4(aPos, 1.0);

}