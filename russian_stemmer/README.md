## Stemmer for texts in Russian with typos

The program takes .csv text file with \t as a delimeter, processes it and extracts stems.

### Requirements
python3 (version 3.5 or higher)  
[jamspell](https://pypi.org/project/jamspell/)  
[pymystem3](https://pypi.org/project/pymystem3/)  
[nltk](https://pypi.org/project/nltk/)  
[langdetect](https://pypi.org/project/langdetect/)  


How to run: python3 runner.py [file.csv]  
Output: the file with stems and words belonging to them.
