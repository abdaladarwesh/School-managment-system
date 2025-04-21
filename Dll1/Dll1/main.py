from flask import Flask, request, jsonify
from flask_cors import CORS  # Import CORS
import my_module

app = Flask(__name__)
CORS(app)  # This will enable CORS for all routes
conn = my_module.connect()

# GET user credentials


@app.route('/users/get/students/<int:user_id>')
def get_students(user_id):
    students = conn.get_students(user_id)
    return students , 200

# Add new user
@app.route('/users/add/student/<string:username>/<string:password>/<string:grade>/<string:class_name>', methods=['PUT'])
def add_student(username, password, grade, class_name):
    try:
        conn.add_student(username, password, grade, class_name)
        return {"Message": "Student added successfully"}, 201
    except Exception as e:
        return jsonify({"error": str(e), "status": 500}), 500
    

@app.route('/users/list/students')
def list_students():
    try:
        students = conn.get_students()
        return jsonify(students) , 200
    except Exception as e:
        return jsonify({"error": str(e), "status": 500}), 500

@app.route('/users/list/teachers')
def list_teachers():
    try:
        teachers = conn.get_teachers()
        return jsonify(teachers) , 200
    except Exception as e:
        return jsonify({"error": str(e), "status": 500}), 500

@app.route('/users/remove/student/<string:username>', methods=['PUT'])
def remove_student(username):
    try:
        conn.remove_student(username)
        return {"Message" : "done"} , 200
    except Exception as e:
        return jsonify({"error": str(e), "status": 500}), 500
    



# sqlplus c##app_user/123@//localhost:1521/XE
# teacher4 emily.davis
@app.route('/users/add/teacher/<string:username>/<string:password>/<string:grade_name>/<string:class_name>/<string:subject_name>', methods=['PUT'])
def add_teacher(username, password, grade_name, class_name, subject_name):
    try:
        conn.add_teacher(username, password, grade_name, class_name, subject_name)
        return {"Message": "Teacher added successfully"}, 201
    except Exception as e:
        return jsonify({"error": str(e), "status": 500}), 500

@app.route('/users/remove/teacher/<string:username>', methods=['PUT'])
def remove_teacher(username):
    try:
        conn.remove_teacher(username)
        return {"Message" : "done"} , 200
    except Exception as e:
        return jsonify({"error": str(e), "status": 500}), 500
    



@app.route('/users/validateCredentials/<string:username>/<string:password>' , methods=['GET'])
def validateCredentials(username , password):
    return {"role":conn.validate_credentials(username, password)} , 200



@app.route('/users/count/student' , methods= ['GET'])
def studentcount():
    return {"count" : conn.count_students()}



@app.route('/users/count/teacher' , methods= ['GET'])
def teachercount():
    return {"count" : conn.count_teachers()}



@app.route('/users/count/class' , methods= ['GET'])
def classcount():
    return {"count" : conn.count_classes()}

@app.route('/teacher/edit/<string:username>/<string:password>/<string:grade_name>/<string:class_name>/<string:subject_name>' , methods= ['PUT'])
def editTeacher(username, password, grade_name, class_name, subject_name):
	conn.edit_teacher(username, password, grade_name, class_name, subject_name)
	return {"Message" : "Teacher updated successfully"} , 200

@app.route('/student/edit/<string:username>/<string:password>/<string:grade_name>/<string:class_name>' , methods= ['PUT'])
def editStudent(username, password, grade_name, class_name):
	conn.edit_student(username, password, grade_name, class_name)
	return {"Message" : "Student updated successfully"} , 200

@app.route('/count/grades' , methods= ['GET'])
def countgrades():
	return conn.get_students_per_grade() , 200

@app.route('/latest/grades/<string:username>' , methods= ['GET'])
def latestgrades(username):
	return conn.get_latest_subject_degrees(username) , 200

@app.route('/avg/grades/<string:username>' , methods= ['GET'])
def avggrades(username):
	return conn.get_latest_avg_degrees(username) , 200
@app.route('/grades/<string:username>' , methods= ['GET'])
def getgrades(username):
	return conn.get_degrees(username) , 200



if __name__ == "__main__":
    app.run(debug=True)