for file in test_data/*.txt
do 
    ./Main ${file} > "out_data/`basename "$file" .txt`.out"
done
