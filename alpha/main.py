import random

file = open("./output.txt", "w")
names = ["erste", "zweite", "dritte", "vierte", "funfte"]
for i in range(5):
    v = []
    for j in range(26):
        letter = random.randint(65, 90)
        while letter in v:
            letter = random.randint(65, 90)
        v.append(letter)
    for j in range(10):
        number = random.randint(48, 57)
        while number in v:
            number = random.randint(48, 57)
        v.append(number)
    random.shuffle(v)
    file.write("vector<char> " + names[i] + " = {")
    for j in v:
        if j != v[-1]:
            file.write("'" + chr(j) + "', ")
        else:
            file.write("'" + chr(j) + "'}")
    file.write(";\n")
