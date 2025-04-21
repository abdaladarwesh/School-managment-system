import requests
import my_module

conn = my_module.connect()



teachers = conn.get_students_per_grade()




print(teachers)