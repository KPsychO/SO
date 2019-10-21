# !/bin/bash

echo "Creating temporal folder ./tmp"
rm -rf tmp/*

echo "Creating ./mountpoint"
rm -rf mount-point/*

echo "Copying fuseLib.c & myFS.h to ./tmp & ./mount-point"
cp scr/fuseLib.c tmp/fuseLib.c
cp src/myFS.h tmp/myFS.h

cp src/fuseLib.c mount-point/fuseLib.c
cp src/myFS.h mount-point/myFS.h

echo "Auditing disk"
./my-fsck-static-64 mount-point

sleep 5

echo "Showing differences between the original files and the copied ones"
if diff tmp/fuseLib.c mount-point/fuseLib.c
then echo "fuseLib.c [EQUALS]"
else echo "fuseLib.c [DIFFERENTS]"
fi

if diff tmp/myFS.h mount-point/myFS.h
then echo "myFS.h [EQUALS]"
else echo "myFS.h [DIFFERENTS]"
fi

sleep 5

echo "Modifiying the size of fuseLib.c on ./mount-point % ./tmp"
truncate mount-point/fuseLib.c -s -1
truncate tmp/fuseLib.c -s -1

echo "Comparing the modified files"
if diff tmp/fuseLib.c mount-point/fuseLib.c
then echo "fuseLib.c [EQUALS]"
else echo "fuseLib.c [DIFFERENTS]"
fi

sleep 5

echo "Creating a new text file in ./tmp and copiying it into ./mount-point"
echo "qwertyuiopasdfgherihuvdcksmlwodjeihnkwmdwojfihebnjklzxcvbnm" > tmp/file.txt
cp tmp/file.txt mount-point/file.txt

echo "Auditing disk"
./my-fsck-static-64 mount-point

sleep 5

echo "Comparing the text files"
if diff tmp/file.txt mount-point/file.txt
then echo "file.txt [EQUALS]"
else echo "file.txt [DIFFERENTS]"
fi

sleep 5

echo "Modifying myFS.h on ./tmp & ./mount-point"
truncate mount-point/myFS.h -s +5
truncate tmp/myFS.h -s +5

echo "Auditing disk"
./my-fsck-static-64 mount-point

sleep 5

echo "Comparing myFS.h on ./tmp & ./mount-point"
if diff tmp/myFS.h mount-point/myFS.h
then echo "myFS.h [EQUALS]"
else echo "myFS.h [DIFFERENTS]"
fi

sleep 5

echo "Ending fusermount"
fusermount -u mount-point
