rm errors.log

cd ./00_test_files/errors
rm -f  *.log  *.am *.ob *.ent *.ext *.warnings *.errors 
cd ..

cd ./00_test_files/mixed
rm -f  *.log  *.am *.ob *.ent *.ext *.warnings *.errors 
cd ..

cd ./00_test_files/valid
rm -f  *.log  *.am *.ob *.ent *.ext *.warnings *.errors 
cd ..

cd ..

make 
./compiler ./00_test_files/errors/randomErrors  ./00_test_files/errors/wrongAddressingMethod ./00_test_files/errors/wrongNumOfCommas  ./00_test_files/errors/wrongNumOfOperands ./00_test_files/errors/wrongUseOfInstructions ./00_test_files/mixed/mixed_01 ./00_test_files/mixed/mixed_02 ./00_test_files/valid/valid_01 ./00_test_files/valid/valid_02
rm -f compiler 
