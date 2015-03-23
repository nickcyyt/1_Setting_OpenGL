
#version 330 core
layout(location = 0) in vec3 in_position;  //set the first input on location (index) 0 ; in_position is our attribute 
layout(location = 1) in vec4 in_color;
 
out vec4 color;

void main()
{
	color = in_color;
	gl_Position = vec4(in_position, 1);  //w is 1.0, also notice cast to a vec4
}