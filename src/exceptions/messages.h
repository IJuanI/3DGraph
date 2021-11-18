#ifndef G3D_EXCEPTIONS_MESSAGES_H_
#define G3D_EXCEPTIONS_MESSAGES_H_

#include <string>

namespace graph3d {
namespace exceptions {

/// Warnings
static const char* WAR001 = "No se definio ninguna carpeta de %ss";
static const char* WAR002 = "No se encontro el archivo %s";
static const char* WAR003 = "No se encontro la carpeta %s";
static const char* WAR004 = "No se encontraron shaders en la carpeta %s. No se cargara el shader.";

/// Errors
static const char* ERR001 = "No se pudo leer el shader %s";
static const char* ERR002 = "Error durante el linkeo de un programa. Shaders involucrados: %s";
static const char* ERR003 = "Error al compilar el shader %s";
static const char* ERR004 = "No se pudo inicializar GLAD";
static const char* ERR005 = "No se pudo crear una ventana de GLFW";
static const char* ERR006 = "El monitor solicitado no existe";
static const char* ERR006_2 = "Monitor solicitado: %d\nCantidad de Monitores: %d";




static const char* ERR010 = "Se solicitó un shader que no está cargado: %s";
/// Internal Interface
const std::string format(const char* const zcFormat, ...);

}  // namespace exceptions
}  // namespace graph3d

#endif