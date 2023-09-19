# TP1-SO

Para la ejecución del programa se debe ingresar el siguiente comando:

./md5 <path>

donde se debe reemplazar <path> por el nombre de archivo o directorio a cuyos archivos se les desee generar el md5.

Para conectar view, existen dos maneras de hacerlo. Por un lado, si se quiere correr md5 y view por separado se debe ingresar en una terminal a parte de aquella en la que se corre el md5 el siguiente comando:

./view /shmMd5

donde “/shmMd5” indica el nombre de la memoria compartida que emplea md5 para que view se pueda conectar a esta. Para iniciar la view de esta manera, se debe tener en cuenta que solo podrá acceder e imprimir los resultados previo a que pasen 2 segundos de iniciado el md5. 
Para correr view y md5 juntos, se debe correr el siguiente comando:

./md5 <path> | ./view 

donde nuevamente se debe reemplazar <path> por el nombre de archivo o directorio a cuyos archivos se les desee generar el md5.
