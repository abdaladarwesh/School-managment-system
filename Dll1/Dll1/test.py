import requests
import my_module

conn = my_module.connect()



teachers = conn.get_teachers()




print(teachers)