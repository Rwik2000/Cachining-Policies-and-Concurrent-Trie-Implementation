# Code to generate workload for Trie data structure
import random
import os

num_words = 1000 #No. of words in each files
len_word = 10 #maximum length of each word
num_threads = 1000 #Total number of files/threads

'''INSERTION WORKLOAD'''
# To create a directory
os.system("mkdir find_test -p")
# Letters to randomly generate and join to make words
letters = ["a","b","c","d","e","f","g","h","i","j","k","l","m",
           "n","o","p","q","r","s","t","u","v","w","x","y","z"]
           
for i in range(num_threads):
    with open("find_test/"+str(i+1)+".txt", "w") as output:
        for j in range(num_words):
            temp_len = random.randint(1 , len_word)
            word = random.choices(letters, k=temp_len)
            word = ("".join(word))
            output.write(word+"\n")
        # end of document
        output.write("--")


'''FIND WORKLOAD'''
# To create a directory
os.system("mkdir insert_test -p")
# Letters to randomly generate and join to make words
letters = ["a","b","c","d","e","f","g","h","i","j","k","l","m",
           "n","o","p","q","r","s","t","u","v","w","x","y","z"]
           
for i in range(num_threads):
    with open("insert_test/"+str(i+1)+".txt", "w") as output:
        for j in range(num_words):
            temp_len = random.randint(1 , len_word)
            word = random.choices(letters, k=temp_len)
            word = ("".join(word))
            output.write(word+"\n")
        # end of document
        output.write("--")


        
        

