# TP1-SO

## Autores
- María Agustina Sanguinetti (63115) - msanguinetti@itba.edu.ar 
- Sol Rodriguez (63029) - solrodriguez@itba.edu.ar 
- Uriel Ángel Arias (63504) - uarias@itba.edu.ar

## Compilación
Para compilar el proyecto, se debe ejecutar el siguiente comando en la terminal desde el directorio del proyecto:

    $ make all

Esto generará los ejecutables de cada archivo en c. Es importante aclarar que es necesario tener instalado el compilador de c, gcc, para que la compilación resulte exitosa. 

## Ejecución
Para la ejecución del programa se debe ingresar el siguiente comando:

    $ ./md5 <path>

donde se debe reemplazar \<path\> por el nombre de archivo o directorio a cuyos archivos se les desee generar el md5.

Para conectar view, existen dos maneras de hacerlo. Por un lado, si se quiere correr md5 y view por separado se debe ingresar en una terminal a parte de aquella en la que se corre el md5 el siguiente comando:

    $ ./view /shmMd5

donde “/shmMd5” indica el nombre de la memoria compartida que emplea md5 para que view se pueda conectar a esta. Para iniciar la view de esta manera se debe tener en cuenta que solo podrá acceder e imprimir los resultados previo a que pasen 2 segundos de iniciado el md5. 

Para correr view  y md5 juntos, se debe correr el siguiente comando:

    $ ./md5 <path> | ./view 

donde nuevamente se debe reemplazar \<path\> por el nombre de archivo o directorio a cuyos archivos se les desee generar el md5.

## Limpieza
Para eliminar los ejecutables generados durante la compilación y el archivo resultante de la ejecución de md5, se pueden utilizar las siguientes opciones:

Eliminar solo el ejecutable view:

    $ make cleanView

Eliminar solo el ejecutable md5:

    $ make cleanApplication

Eliminar solo el ejecutable slave:

    $ make cleanSlave

Eliminar el archivo resutante de la ejecución de md5:

    $ make cleanResult
    
Eliminar todos los ejecutables y el archivo resultante de la ejecución de md5: 

    $ make cleanAll
