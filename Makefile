# source files
SRCS := \
    Main.cpp \
	Shader.cpp \
	Mesh.cpp \
	Window.cpp \
	Camera.cpp \
	stb_image.cpp \
	Texture.cpp \
	Light.cpp \
	DirectionalLight.cpp \
	PointLight.cpp \
	SpotLight.cpp \
	ShadowMap.cpp \
	OmniShadowMap.cpp \
	Material.cpp \
	Model.cpp \
	SkyBox.cpp

ifeq ($(OS),Windows_NT)
	include win.mak
else
	include linux.mak
endif