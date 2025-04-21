import requests
import my_module

conn = my_module.connect()



teachers = conn.get_latest_subject_degrees("emily.davis")




print(teachers)