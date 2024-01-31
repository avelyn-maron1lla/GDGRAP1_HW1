#version 330 core

out vec4 FragColor; //returns a color
//Simple shader that colors the model Red
void main()
{
	// vector elements in order RGBA (ranging from 1 to 0)
	FragColor = vec4(0.7f, 0.5f, 0.18f, 1.0f); //setting color of the fragment
}