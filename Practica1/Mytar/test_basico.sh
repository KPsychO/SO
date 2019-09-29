#!/bin/bash

# STATUS:
# 0 - No errors
# 1 - Program not working as specified
# 2 - Program not found

# 1. Comprobará que el programa mytar está en el directorio actual y que es ejecutable. En caso contrario mostrará un mensaje informativo por pantalla y terminará.
if [ ! -x ./mytar ]; then
    echo "No existe el ejecutable 'mytar'.\n" >&2
    exit 2
fi

# 2. Comprobará si existe un directorio tmp dentro del directorio actual. Si existe lo borrará, incluyendo todo lo que haya dentro de él (mirar la opción -r del comando rm).
[ -d tmp ] && rm -r tmp

# 3. Creará un nuevo directorio temporal tmp dentro del directorio actual y cambiará a este directorio.
mkdir tmp && cd tmp

# 4. Creará tres ficheros (dentro del directorio):

# file1.txt: con el contenido “Hello world!”, utilizando la orden echo y redirigiendo la salida al fichero.
echo "Hello World!" > file1.txt
# file2.txt: con una copia de las 10 primeras líneas del fichero /etc/passwd. Se hace fácil utilizando el programa head y redirigiendo la salida al fichero.
head -n10 /etc/passwd > file2.txt
# file3.dat: con un contenido binario aleatorio de 1024 bytes, tomado del dispositivo /dev/urandom. De nuevo conviene utilizar head con la opción -c.
head -c1024 /dev/urandom > file3.dat

farray=(file1.txt file2.txt file3.dat)

# 5. Invocará el programa mytar que hemos desarrollado, para crear un fichero filetar.mtar con el contenido de los tres ficheros anteriores.
../mytar -cf tarfile.mtar ${farray[@]} || { echo "Error creando mytar.\n" >&2; exit 1; }
[ -f tarfile.mtar ] || { echo "No se ha podido crear el fichero.\n"; exit 1; }

# 6 Creará un directorio out (dentro del directorio actual, que debe ser tmp) y copiará el fichero filetar.mtar al nuevo directorio.
mkdir out || { echo "Error creando el directorio 'out'.\n"; exit 1; }
cp tarfile.mtar out/

# 7. Cambiará al directorio out y ejecutará el programa mytar para extraer el contenido del tarball.
cd out
../../mytar -xf tarfile.mtar || { echo "Error while extracting mytar" >&2; exit 1; }

# 8. Usará el programa diff para comparar los fichero extraídos con los originales, que estarán en el directorio anterior (..).
for i in "${farray[@]}"; do
    echo diff "../$i" "$i"
    diff "../$i" "$i" || exit 1
done;

# 9. Si los tres ficheros extraídos son iguales que los originales, volverá al directorio original (../..), mostrará el mensaje “Correct” por pantalla y devolverá 0. Si hay algún error, volverá al directorio original, mostrará un mensaje descriptivo por pantalla y devolverá 1.
echo "Correct"
exit 0