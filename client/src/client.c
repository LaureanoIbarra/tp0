#include "client.h"
#include <readline/readline.h>
#include <string.h>
#include <utils.h>

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger;
	t_config* config;

	/* ---------------- LOGGING ---------------- */

	logger = iniciar_logger();
	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"
	log_info(logger, "PUNTO 1, LOGGER");
	log_info(logger, "Hola! Soy un log\n");
	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();

	// Usando el config creado previamente, leemos los valores del config y los 
	// dejamos en las variables 'ip', 'puerto' y 'valor'

	valor = config_get_string_value(config, "CLAVE");
	ip = config_get_string_value(config, "IP");
	puerto = config_get_string_value(config, "PUERTO");

	// Loggeamos el valor de config

	log_info(logger, "el valor es: %s", valor);
	log_info(logger, "la ip es: %s", ip);
	log_info(logger, "el puerto es: %s\n", puerto);


	/* ---------------- LEER DE CONSOLA ---------------- */
	log_info(logger, "PUNTO 2, LEYENDO DE CONSOLA");
	leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	log_info(logger, "PUNTO 3, CONEXION CON SERVIDOR");

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);

	// Enviamos al servidor el valor de CLAVE como mensaje
	enviar_mensaje(valor, conexion);

	// Armamos y enviamos el paquete
	paquete(conexion);

	terminar_programa(conexion, logger, config);

}

t_log* iniciar_logger(void)
{
	t_log * logger = log_create("CLIENTE.log", "CLIENTE_LOG", 1, LOG_LEVEL_INFO);
	if (logger == NULL){
		printf("No se pudo crear el logger\n");
		exit(EXIT_FAILURE);
	}
	return logger;
	
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config = config_create("./cliente.config");
	if (nuevo_config == NULL){
		printf("No se pudo crear el config\n");
		exit(EXIT_FAILURE);
	}
	return nuevo_config;
}

void leer_consola(t_log* logger)
{
	char* leido;

	// La primera te la dejo de yapa
	// El resto, las vamos leyendo y logueando hasta recibir un string vacío
	// ¡No te olvides de liberar las lineas antes de regresar!

	while(1){
		leido = readline("> ");
		if (!strncmp(leido, "", 4))
		{
			break;
		}
		log_info(logger, leido);
		free(leido);
		
	}
}

void paquete(int conexion)
{
	// Ahora toca lo divertido!
	char* leido = NULL;
	t_paquete* paquete = crear_paquete();


	while(1){
		leido = readline("> ");
		if (!strncmp(leido, "", 4))
		{
			break;
		}
		agregar_a_paquete(paquete, leido, strlen(leido) + 1);
		free(leido);
	}
	
	// Leemos y esta vez agregamos las lineas al paquete
	// agregar_a_paquete(paquete, )

	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	enviar_paquete(paquete, conexion);
	eliminar_paquete(paquete);
	
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	if (logger != NULL)
	{
		log_destroy(logger);
	}
	if (config != NULL){
		config_destroy(config);
	}
	
	liberar_conexion(conexion);
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */
}
