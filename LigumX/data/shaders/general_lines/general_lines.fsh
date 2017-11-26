#version 410 core

#define PROVIDER_Material

// Include Providers Marker

out vec3 color;


void main()
{
    color = g_Material.m_DiffuseColor;
}