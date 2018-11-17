import stemmer as st
import data_reader as dr

from sys import argv

data = dr.get_data(argv[1])
stemmer = st.StemmerRus()

for string in data:
    stemmer.stem(string)

stemmer.save_results_to_file('stems.txt')
