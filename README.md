# CrimeLookupHash
CS 251 Data Structures Project

Instructions:
    Refer to the "Crimes.csv" to find a valid case number to enter. 

    Sample Input: R08U3503
    Sample Output:
        >> hash index: 2283503 <<
        R08U3503:
            date/time: 11/01/2014 09:00:00 AM
            city area: 9 => Near North Side
            IUCR code: 1152
            arrested: false

Objective:
    The purpose of this project was to use a hashtable to store and lookup crime data by case number while decreasing number of collisions. In this project, a hash table is created from CSV files via htttps://data.cityofchicago.org. Two files that are parsed include the "Crimes.csv" file (which contains crime data from the city of Chicagho during the years 2013 - 2015) and the "Areas.csv" file (contains numeric location of the crime to the name of the community area).
