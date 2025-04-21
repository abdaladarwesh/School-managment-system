#include <pybind11/pybind11.h>
#include "nanodbc.h"
#include <string>
#include <vector>

namespace py = pybind11;

class OracleConnector {
public:
	OracleConnector(const std::string& connection_string)
		: conn_(connection_string) {}

	py::list execute_query(const std::string& query) {
		py::list results;
		try {
			nanodbc::result result = execute(conn_, query);
			const short columns = result.columns();

			while (result.next()) {
				py::dict row;
				for (short i = 0; i < columns; ++i) {
					std::string col_name = result.column_name(i);
					try {
						if (result.is_null(i)) {
							row[col_name.c_str()] = py::none();
						}
						else {
							row[col_name.c_str()] = result.get<std::string>(i);
						}
					}
					catch (...) {
						row[col_name.c_str()] = "CONVERSION_ERROR";
					}
				}
				results.append(row);
			}
		}
		catch (const std::exception& e) {
			throw std::runtime_error("Database error: " + std::string(e.what()));
		}
		return results;
	}

	int insert(const std::string& table_name, const py::dict& data) {
		try {
			// Build the INSERT query
			std::string columns;
			std::string values;

			for (auto item : data) {
				std::string column = py::cast<std::string>(item.first);
				std::string value = py::cast<std::string>(item.second);

				if (!columns.empty()) {
					columns += ", ";
					values += ", ";
				}
				columns += column;
				values += "'" + value + "'";
			}

			// Execute the insert
			std::string query = "INSERT INTO " + table_name +
				" (" + columns + ") VALUES (" + values + ")";
			execute(conn_, query);

			// In Oracle, we can assume 1 row affected for single-row inserts
			return 1;
		}
		catch (const std::exception& e) {
			throw std::runtime_error("Insert error: " + std::string(e.what()));
		}
	}
	std::string get_version() {
		auto results = execute_query("SELECT * FROM v$version");
		if (len(results) > 0) {
			return py::cast<std::string>(results[0]["BANNER"]);
		}
		return "Version information not available";
	}
	std::string validateCredentials(const std::string& username, const std::string& password) {
		try {
			// First query to check user credentials
			nanodbc::statement user_stmt(conn_);
			user_stmt.prepare("SELECT user_id FROM users WHERE username = ? AND password = ?");
			user_stmt.bind(0, username.c_str());
			user_stmt.bind(1, password.c_str());
			nanodbc::result user_result = user_stmt.execute();

			if (user_result.next()) {
				int user_id = user_result.get<int>(0);

				// 1. First check if user is an admin
				nanodbc::statement admin_stmt(conn_);
				admin_stmt.prepare("SELECT * FROM admins WHERE user_id = ?");
				admin_stmt.bind(0, &user_id);
				nanodbc::result admin_result = admin_stmt.execute();

				if (admin_result.next()) {
					return "admin";
				}

				// 2. Check if user is a teacher
				nanodbc::statement teacher_stmt(conn_);
				teacher_stmt.prepare("SELECT * FROM teachers WHERE user_id = ?");
				teacher_stmt.bind(0, &user_id);
				nanodbc::result teacher_result = teacher_stmt.execute();

				if (teacher_result.next()) {
					return "teacher";
				}

				// 3. Check if user is a student
				nanodbc::statement student_stmt(conn_);
				student_stmt.prepare("SELECT * FROM students WHERE user_id = ?");
				student_stmt.bind(0, &user_id);
				nanodbc::result student_result = student_stmt.execute();

				if (student_result.next()) {
					return "student";
				}
			}
			return ""; // No matching user found or invalid credentials
		}
		catch (const std::exception& e) {
			throw std::runtime_error("Validation error: " + std::string(e.what()));
		}
	}
	int count_students() {
		try {
			nanodbc::result result = execute(conn_, "SELECT COUNT(*) FROM students");
			if (result.next()) {
				return result.get<int>(0);
			}
			return 0;
		}
		catch (const std::exception& e) {
			throw std::runtime_error("Count error: " + std::string(e.what()));
		}
	}
	int count_teachers() {
		try {
			nanodbc::result result = execute(conn_, "SELECT COUNT(*) FROM teachers");
			if (result.next()) {
				return result.get<int>(0);
			}
			return 0;
		}
		catch (const std::exception& e) {
			throw std::runtime_error("Count error: " + std::string(e.what()));
		}
	}
	int count_classes() {
		try {
			nanodbc::result result = execute(conn_, "SELECT COUNT(*) FROM classes");
			if (result.next()) {
				return result.get<int>(0);
			}
			return 0;
		}
		catch (const std::exception& e) {
			throw std::runtime_error("Count error: " + std::string(e.what()));
		}
	}
	void remove_student(const std::string& username) {
		nanodbc::transaction transaction(conn_);
		try {
			// First get the user_id
			nanodbc::statement id_stmt(conn_);
			id_stmt.prepare("SELECT user_id FROM users WHERE username = ?");
			id_stmt.bind(0, username.c_str());
			nanodbc::result id_result = id_stmt.execute();

			if (!id_result.next()) {
				throw std::runtime_error("Student not found with username: " + username);
			}

			int user_id = id_result.get<int>(0);

			// Delete from students table
			nanodbc::statement student_stmt(conn_);
			student_stmt.prepare("DELETE FROM students WHERE user_id = ?");
			student_stmt.bind(0, &user_id);
			student_stmt.execute();

			// Delete from users table
			nanodbc::statement user_stmt(conn_);
			user_stmt.prepare("DELETE FROM users WHERE user_id = ?");
			user_stmt.bind(0, &user_id);
			user_stmt.execute();

			transaction.commit();
		}
		catch (...) {
			transaction.rollback();
			throw;
		}
	}

	void remove_teacher(const std::string& username) {
		nanodbc::transaction transaction(conn_);
		try {
			// First get the user_id
			nanodbc::statement id_stmt(conn_);
			id_stmt.prepare("SELECT user_id FROM users WHERE username = ?");
			id_stmt.bind(0, username.c_str());
			nanodbc::result id_result = id_stmt.execute();

			if (!id_result.next()) {
				throw std::runtime_error("Teacher not found with username: " + username);
			}

			int user_id = id_result.get<int>(0);

			// Delete from teachers table
			nanodbc::statement teacher_stmt(conn_);
			teacher_stmt.prepare("DELETE FROM teachers WHERE user_id = ?");
			teacher_stmt.bind(0, &user_id);
			teacher_stmt.execute();

			// Delete from users table
			nanodbc::statement user_stmt(conn_);
			user_stmt.prepare("DELETE FROM users WHERE user_id = ?");
			user_stmt.bind(0, &user_id);
			user_stmt.execute();

			transaction.commit();
		}
		catch (...) {
			transaction.rollback();
			throw;
		}
	}
	void add_student(const std::string& username, const std::string& password,
		const std::string& grade_name, const std::string& class_name) {
		try {
			// Start transaction
			nanodbc::transaction transaction(conn_);

			// 1. Insert into users table
			nanodbc::statement user_stmt(conn_);
			user_stmt.prepare("INSERT INTO users (username, password) VALUES (?, ?)");
			user_stmt.bind(0, username.c_str());
			user_stmt.bind(1, password.c_str());
			user_stmt.execute();

			// 2. Get the generated user_id
			int user_id = 0;
			{
				nanodbc::statement id_stmt(conn_);
				id_stmt.prepare("SELECT user_id FROM users WHERE username = ?");
				id_stmt.bind(0, username.c_str());
				nanodbc::result id_result = id_stmt.execute();

				if (!id_result.next()) {
					throw std::runtime_error("Failed to retrieve new user ID");
				}
				user_id = id_result.get<int>(0);
			}

			// 3. Handle grade (insert if not exists)
			int grade_id = 0;
			{
				// Try to find existing grade
				nanodbc::statement find_grade(conn_);
				find_grade.prepare("SELECT grade_id FROM grade WHERE grade_name = ?");
				find_grade.bind(0, grade_name.c_str());
				nanodbc::result grade_result = find_grade.execute();

				if (grade_result.next()) {
					grade_id = grade_result.get<int>(0);
				}
				else {
					// Insert new grade
					nanodbc::statement insert_grade(conn_);
					insert_grade.prepare("INSERT INTO grade (grade_name) VALUES (?)");
					insert_grade.bind(0, grade_name.c_str());
					insert_grade.execute();

					// Get the new grade_id
					nanodbc::statement get_grade_id(conn_);
					get_grade_id.prepare("SELECT grade_id FROM grade WHERE grade_name = ?");
					get_grade_id.bind(0, grade_name.c_str());
					nanodbc::result new_grade_result = get_grade_id.execute();

					if (!new_grade_result.next()) {
						throw std::runtime_error("Failed to retrieve new grade ID");
					}
					grade_id = new_grade_result.get<int>(0);
				}
			}

			// 4. Handle class (insert if not exists)
			int class_id = 0;
			{
				// Try to find existing class
				nanodbc::statement find_class(conn_);
				find_class.prepare("SELECT class_id FROM classes WHERE name = ?");
				find_class.bind(0, class_name.c_str());
				nanodbc::result class_result = find_class.execute();

				if (class_result.next()) {
					class_id = class_result.get<int>(0);
				}
				else {
					// Insert new class
					nanodbc::statement insert_class(conn_);
					insert_class.prepare("INSERT INTO classes (name) VALUES (?)");
					insert_class.bind(0, class_name.c_str());
					insert_class.execute();

					// Get the new class_id
					nanodbc::statement get_class_id(conn_);
					get_class_id.prepare("SELECT class_id FROM classes WHERE name = ?");
					get_class_id.bind(0, class_name.c_str());
					nanodbc::result new_class_result = get_class_id.execute();

					if (!new_class_result.next()) {
						throw std::runtime_error("Failed to retrieve new class ID");
					}
					class_id = new_class_result.get<int>(0);
				}
			}

			// 5. Insert into students table
			nanodbc::statement student_stmt(conn_);
			student_stmt.prepare("INSERT INTO students (user_id, class_id, grade_id) VALUES (?, ?, ?)");
			student_stmt.bind(0, &user_id);
			student_stmt.bind(1, &class_id);
			student_stmt.bind(2, &grade_id);
			student_stmt.execute();

			// Commit transaction if everything succeeded
			transaction.commit();
		}
		catch (const std::exception& e) {
			// Transaction will automatically roll back when going out of scope
			throw std::runtime_error("Add student error: " + std::string(e.what()));
		}
	}
	void add_teacher(const std::string& username, const std::string& password,
		const std::string& grade_name, const std::string& class_name,
		const std::string& subject_name) {
		try {
			// Start transaction
			nanodbc::transaction transaction(conn_);

			// 1. Insert into users table
			nanodbc::statement user_stmt(conn_);
			user_stmt.prepare("INSERT INTO users (username, password) VALUES (?, ?)");
			user_stmt.bind(0, username.c_str());
			user_stmt.bind(1, password.c_str());
			user_stmt.execute();

			// 2. Get the generated user_id
			int user_id = 0;
			{
				nanodbc::statement id_stmt(conn_);
				id_stmt.prepare("SELECT user_id FROM users WHERE username = ?");
				id_stmt.bind(0, username.c_str());
				nanodbc::result id_result = id_stmt.execute();

				if (!id_result.next()) {
					throw std::runtime_error("Failed to retrieve new user ID");
				}
				user_id = id_result.get<int>(0);
			}

			// 3. Insert into teachers table
			int teacher_id = 0;
			{
				nanodbc::statement teacher_stmt(conn_);
				teacher_stmt.prepare("INSERT INTO teachers (user_id) VALUES (?)");
				teacher_stmt.bind(0, &user_id);
				teacher_stmt.execute();

				// Get the generated teacher_id
				nanodbc::statement tid_stmt(conn_);
				tid_stmt.prepare("SELECT teacher_id FROM teachers WHERE user_id = ?");
				tid_stmt.bind(0, &user_id);
				nanodbc::result tid_result = tid_stmt.execute();

				if (!tid_result.next()) {
					throw std::runtime_error("Failed to retrieve new teacher ID");
				}
				teacher_id = tid_result.get<int>(0);
			}

			// 4. Handle grade (insert if not exists)
			int grade_id = 0;
			{
				// Try to find existing grade
				nanodbc::statement find_grade(conn_);
				find_grade.prepare("SELECT grade_id FROM grade WHERE grade_name = ?");
				find_grade.bind(0, grade_name.c_str());
				nanodbc::result grade_result = find_grade.execute();

				if (grade_result.next()) {
					grade_id = grade_result.get<int>(0);
				}
				else {
					// Insert new grade
					nanodbc::statement insert_grade(conn_);
					insert_grade.prepare("INSERT INTO grade (grade_name) VALUES (?)");
					insert_grade.bind(0, grade_name.c_str());
					insert_grade.execute();

					// Get the new grade_id
					nanodbc::statement get_grade_id(conn_);
					get_grade_id.prepare("SELECT grade_id FROM grade WHERE grade_name = ?");
					get_grade_id.bind(0, grade_name.c_str());
					nanodbc::result new_grade_result = get_grade_id.execute();

					if (!new_grade_result.next()) {
						throw std::runtime_error("Failed to retrieve new grade ID");
					}
					grade_id = new_grade_result.get<int>(0);
				}

				// Link teacher to grade
				nanodbc::statement teacher_grade(conn_);
				teacher_grade.prepare("INSERT INTO teacher_with_grade (teacher_id, grade_id) VALUES (?, ?)");
				teacher_grade.bind(0, &teacher_id);
				teacher_grade.bind(1, &grade_id);
				teacher_grade.execute();
			}

			// 5. Handle class (insert if not exists)
			int class_id = 0;
			{
				// Try to find existing class
				nanodbc::statement find_class(conn_);
				find_class.prepare("SELECT class_id FROM classes WHERE name = ?");
				find_class.bind(0, class_name.c_str());
				nanodbc::result class_result = find_class.execute();

				if (class_result.next()) {
					class_id = class_result.get<int>(0);
				}
				else {
					// Insert new class
					nanodbc::statement insert_class(conn_);
					insert_class.prepare("INSERT INTO classes (name) VALUES (?)");
					insert_class.bind(0, class_name.c_str());
					insert_class.execute();

					// Get the new class_id
					nanodbc::statement get_class_id(conn_);
					get_class_id.prepare("SELECT class_id FROM classes WHERE name = ?");
					get_class_id.bind(0, class_name.c_str());
					nanodbc::result new_class_result = get_class_id.execute();

					if (!new_class_result.next()) {
						throw std::runtime_error("Failed to retrieve new class ID");
					}
					class_id = new_class_result.get<int>(0);
				}

				// Link teacher to class
				nanodbc::statement teacher_class(conn_);
				teacher_class.prepare("INSERT INTO class_with_teacher (teacher_id, class_id) VALUES (?, ?)");
				teacher_class.bind(0, &teacher_id);
				teacher_class.bind(1, &class_id);
				teacher_class.execute();
			}

			// 6. Handle subject (insert if not exists)
			{
				// Try to find existing subject
				nanodbc::statement find_subject(conn_);
				find_subject.prepare("SELECT sub_id FROM subjects WHERE sub_name = ?");
				find_subject.bind(0, subject_name.c_str());
				nanodbc::result subject_result = find_subject.execute();

				if (!subject_result.next()) {
					// Insert new subject
					nanodbc::statement insert_subject(conn_);
					insert_subject.prepare("INSERT INTO subjects (sub_name, teacher_id) VALUES (?, ?)");
					insert_subject.bind(0, subject_name.c_str());
					insert_subject.bind(1, &teacher_id);
					insert_subject.execute();
				}

				// Note: If subject exists, we don't change its teacher assignment
			}

			// Commit transaction if everything succeeded
			transaction.commit();
		}
		catch (const std::exception& e) {
			throw std::runtime_error("Add teacher error: " + std::string(e.what()));
		}
	}
	py::list get_students() {
		try {
			py::list students_list;

			nanodbc::statement stmt(conn_);
			stmt.prepare(
				"SELECT u.username, c.name AS class_name, g.grade_name "
				"FROM students s "
				"JOIN users u ON s.USER_ID = u.user_id "
				"LEFT JOIN classes c ON s.CLASS_ID = c.class_id "
				"LEFT JOIN grade g ON s.GRADE_ID = g.grade_id"
			);

			nanodbc::result result = stmt.execute();

			while (result.next()) {
				py::dict student_info;

				// Username (always required)
				student_info["username"] = result.get<std::string>(0);

				// Class name (may be NULL)
				try {
					student_info["class_name"] = result.get<std::string>(1);
				}
				catch (...) {
					student_info["class_name"] = py::none();
				}

				// Grade name (may be NULL)
				try {
					student_info["grade_name"] = result.get<std::string>(2);
				}
				catch (...) {
					student_info["grade_name"] = py::none();
				}

				students_list.append(student_info);
			}

			if (len(students_list) == 0) {
				throw std::runtime_error("No students found");
			}

			return students_list;
		}
		catch (const std::exception& e) {
			throw std::runtime_error("Get students error: " + std::string(e.what()));
		}
	}
	py::list get_teachers() {
		try {
			py::list teachers_list;

			nanodbc::statement stmt(conn_);
			stmt.prepare(
				"SELECT u.username, "
				"LISTAGG(DISTINCT g.grade_name, ', ') WITHIN GROUP (ORDER BY g.grade_name) AS grades, "
				"LISTAGG(DISTINCT sub.sub_name, ', ') WITHIN GROUP (ORDER BY sub.sub_name) AS subjects, "
				"LISTAGG(DISTINCT c.name, ', ') WITHIN GROUP (ORDER BY c.name) AS classes "
				"FROM teachers t "
				"JOIN users u ON t.USER_ID = u.user_id "
				"LEFT JOIN teacher_with_grade twg ON t.teacher_id = twg.teacher_id "
				"LEFT JOIN grade g ON twg.grade_id = g.grade_id "
				"LEFT JOIN subjects sub ON t.teacher_id = sub.teacher_id "
				"LEFT JOIN class_with_teacher cwt ON t.teacher_id = cwt.teacher_id "
				"LEFT JOIN classes c ON cwt.class_id = c.class_id "
				"GROUP BY u.username"
			);

			nanodbc::result result = stmt.execute();

			while (result.next()) {
				py::dict teacher_info;

				// Username (always required)
				teacher_info["username"] = result.get<std::string>(0);

				// Grades they teach (may be NULL)
				try {
					teacher_info["grades"] = result.get<std::string>(1);
				}
				catch (...) {
					teacher_info["grades"] = py::none();
				}

				// Subjects they teach (may be NULL)
				try {
					teacher_info["subjects"] = result.get<std::string>(2);
				}
				catch (...) {
					teacher_info["subjects"] = py::none();
				}

				// Classes they teach (may be NULL)
				try {
					teacher_info["classes"] = result.get<std::string>(3);
				}
				catch (...) {
					teacher_info["classes"] = py::none();
				}

				teachers_list.append(teacher_info);
			}

			if (len(teachers_list) == 0) {
				throw std::runtime_error("No teachers found");
			}

			return teachers_list;
		}
		catch (const std::exception& e) {
			throw std::runtime_error("Get teachers error: " + std::string(e.what()));
		}
	}
	void edit_teacher(const std::string& username, const std::string& password,
		const std::string& grade_name, const std::string& class_name,
		const std::string& subject_name) {
		try {
			// Start transaction
			nanodbc::transaction transaction(conn_);

			// 1. Get the user_id for the teacher
			int user_id = 0;
			{
				nanodbc::statement id_stmt(conn_);
				id_stmt.prepare("SELECT user_id FROM users WHERE username = ?");
				id_stmt.bind(0, username.c_str());
				nanodbc::result id_result = id_stmt.execute();

				if (!id_result.next()) {
					throw std::runtime_error("Teacher not found with username: " + username);
				}
				user_id = id_result.get<int>(0);
			}

			// 2. Update password if provided
			if (!password.empty()) {
				nanodbc::statement pwd_stmt(conn_);
				pwd_stmt.prepare("UPDATE users SET password = ? WHERE user_id = ?");
				pwd_stmt.bind(0, password.c_str());
				pwd_stmt.bind(1, &user_id);
				pwd_stmt.execute();
			}

			// 3. Get teacher_id
			int teacher_id = 0;
			{
				nanodbc::statement tid_stmt(conn_);
				tid_stmt.prepare("SELECT teacher_id FROM teachers WHERE user_id = ?");
				tid_stmt.bind(0, &user_id);
				nanodbc::result tid_result = tid_stmt.execute();

				if (!tid_result.next()) {
					throw std::runtime_error("Teacher record not found for user: " + username);
				}
				teacher_id = tid_result.get<int>(0);
			}

			// 4. Handle grade if provided
			if (!grade_name.empty()) {
				int grade_id = 0;
				// Try to find existing grade
				nanodbc::statement find_grade(conn_);
				find_grade.prepare("SELECT grade_id FROM grade WHERE grade_name = ?");
				find_grade.bind(0, grade_name.c_str());
				nanodbc::result grade_result = find_grade.execute();

				if (grade_result.next()) {
					grade_id = grade_result.get<int>(0);
				}
				else {
					// Insert new grade
					nanodbc::statement insert_grade(conn_);
					insert_grade.prepare("INSERT INTO grade (grade_name) VALUES (?)");
					insert_grade.bind(0, grade_name.c_str());
					insert_grade.execute();

					// Get the new grade_id
					nanodbc::statement get_grade_id(conn_);
					get_grade_id.prepare("SELECT grade_id FROM grade WHERE grade_name = ?");
					get_grade_id.bind(0, grade_name.c_str());
					nanodbc::result new_grade_result = get_grade_id.execute();

					if (!new_grade_result.next()) {
						throw std::runtime_error("Failed to retrieve new grade ID");
					}
					grade_id = new_grade_result.get<int>(0);
				}

				// Check if teacher already has a grade assignment
				nanodbc::statement check_grade(conn_);
				check_grade.prepare("SELECT COUNT(*) FROM teacher_with_grade WHERE teacher_id = ?");
				check_grade.bind(0, &teacher_id);
				nanodbc::result check_result = check_grade.execute();
				check_result.next();
				int grade_count = check_result.get<int>(0);

				if (grade_count > 0) {
					// Update existing grade assignment
					nanodbc::statement update_grade(conn_);
					update_grade.prepare("UPDATE teacher_with_grade SET grade_id = ? WHERE teacher_id = ?");
					update_grade.bind(0, &grade_id);
					update_grade.bind(1, &teacher_id);
					update_grade.execute();
				}
				else {
					// Insert new grade assignment
					nanodbc::statement teacher_grade(conn_);
					teacher_grade.prepare("INSERT INTO teacher_with_grade (teacher_id, grade_id) VALUES (?, ?)");
					teacher_grade.bind(0, &teacher_id);
					teacher_grade.bind(1, &grade_id);
					teacher_grade.execute();
				}
			}

			// 5. Handle class if provided
			if (!class_name.empty()) {
				int class_id = 0;
				// Try to find existing class
				nanodbc::statement find_class(conn_);
				find_class.prepare("SELECT class_id FROM classes WHERE name = ?");
				find_class.bind(0, class_name.c_str());
				nanodbc::result class_result = find_class.execute();

				if (class_result.next()) {
					class_id = class_result.get<int>(0);
				}
				else {
					// Insert new class
					nanodbc::statement insert_class(conn_);
					insert_class.prepare("INSERT INTO classes (name) VALUES (?)");
					insert_class.bind(0, class_name.c_str());
					insert_class.execute();

					// Get the new class_id
					nanodbc::statement get_class_id(conn_);
					get_class_id.prepare("SELECT class_id FROM classes WHERE name = ?");
					get_class_id.bind(0, class_name.c_str());
					nanodbc::result new_class_result = get_class_id.execute();

					if (!new_class_result.next()) {
						throw std::runtime_error("Failed to retrieve new class ID");
					}
					class_id = new_class_result.get<int>(0);
				}

				// Check if teacher already has a class assignment
				nanodbc::statement check_class(conn_);
				check_class.prepare("SELECT COUNT(*) FROM class_with_teacher WHERE teacher_id = ?");
				check_class.bind(0, &teacher_id);
				nanodbc::result check_result = check_class.execute();
				check_result.next();
				int class_count = check_result.get<int>(0);

				if (class_count > 0) {
					// Update existing class assignment
					nanodbc::statement update_class(conn_);
					update_class.prepare("UPDATE class_with_teacher SET class_id = ? WHERE teacher_id = ?");
					update_class.bind(0, &class_id);
					update_class.bind(1, &teacher_id);
					update_class.execute();
				}
				else {
					// Insert new class assignment
					nanodbc::statement teacher_class(conn_);
					teacher_class.prepare("INSERT INTO class_with_teacher (teacher_id, class_id) VALUES (?, ?)");
					teacher_class.bind(0, &teacher_id);
					teacher_class.bind(1, &class_id);
					teacher_class.execute();
				}
			}

			// 6. Handle subject if provided
			if (!subject_name.empty()) {
				// Check if subject already exists
				nanodbc::statement find_subject(conn_);
				find_subject.prepare("SELECT sub_id FROM subjects WHERE sub_name = ?");
				find_subject.bind(0, subject_name.c_str());
				nanodbc::result subject_result = find_subject.execute();

				if (subject_result.next()) {
					// Update existing subject to assign to this teacher
					int sub_id = subject_result.get<int>(0);
					nanodbc::statement update_subject(conn_);
					update_subject.prepare("UPDATE subjects SET teacher_id = ? WHERE sub_id = ?");
					update_subject.bind(0, &teacher_id);
					update_subject.bind(1, &sub_id);
					update_subject.execute();
				}
				else {
					// Check if teacher already has a subject
					nanodbc::statement check_subject(conn_);
					check_subject.prepare("SELECT sub_id FROM subjects WHERE teacher_id = ?");
					check_subject.bind(0, &teacher_id);
					nanodbc::result check_result = check_subject.execute();

					if (check_result.next()) {
						// Update existing subject name
						int sub_id = check_result.get<int>(0);
						nanodbc::statement update_subject_name(conn_);
						update_subject_name.prepare("UPDATE subjects SET sub_name = ? WHERE sub_id = ?");
						update_subject_name.bind(0, subject_name.c_str());
						update_subject_name.bind(1, &sub_id);
						update_subject_name.execute();
					}
					else {
						// Insert new subject
						nanodbc::statement insert_subject(conn_);
						insert_subject.prepare("INSERT INTO subjects (sub_name, teacher_id) VALUES (?, ?)");
						insert_subject.bind(0, subject_name.c_str());
						insert_subject.bind(1, &teacher_id);
						insert_subject.execute();
					}
				}
			}

			// Commit transaction if everything succeeded
			transaction.commit();
		}
		catch (const std::exception& e) {
			throw std::runtime_error("Edit teacher error: " + std::string(e.what()));
		}
	}
	void edit_student(const std::string& username, const std::string& password,
		const std::string& grade_name, const std::string& class_name) {
		try {
			// Start transaction
			nanodbc::transaction transaction(conn_);

			// 1. Get the user_id for the student
			int user_id = 0;
			{
				nanodbc::statement id_stmt(conn_);
				id_stmt.prepare("SELECT user_id FROM users WHERE username = ?");
				id_stmt.bind(0, username.c_str());
				nanodbc::result id_result = id_stmt.execute();

				if (!id_result.next()) {
					throw std::runtime_error("Student not found with username: " + username);
				}
				user_id = id_result.get<int>(0);
			}

			// 2. Update password if provided
			if (!password.empty()) {
				nanodbc::statement pwd_stmt(conn_);
				pwd_stmt.prepare("UPDATE users SET password = ? WHERE user_id = ?");
				pwd_stmt.bind(0, password.c_str());
				pwd_stmt.bind(1, &user_id);
				pwd_stmt.execute();
			}

			// 3. Get student record
			int student_id = 0;
			{
				nanodbc::statement student_stmt(conn_);
				student_stmt.prepare("SELECT student_id FROM students WHERE user_id = ?");
				student_stmt.bind(0, &user_id);
				nanodbc::result student_result = student_stmt.execute();

				if (!student_result.next()) {
					throw std::runtime_error("Student record not found for user: " + username);
				}
				student_id = student_result.get<int>(0);
			}

			// 4. Handle grade if provided
			if (!grade_name.empty()) {
				int grade_id = 0;
				// Try to find existing grade
				nanodbc::statement find_grade(conn_);
				find_grade.prepare("SELECT grade_id FROM grade WHERE grade_name = ?");
				find_grade.bind(0, grade_name.c_str());
				nanodbc::result grade_result = find_grade.execute();

				if (grade_result.next()) {
					grade_id = grade_result.get<int>(0);
				}
				else {
					// Insert new grade
					nanodbc::statement insert_grade(conn_);
					insert_grade.prepare("INSERT INTO grade (grade_name) VALUES (?)");
					insert_grade.bind(0, grade_name.c_str());
					insert_grade.execute();

					// Get the new grade_id
					nanodbc::statement get_grade_id(conn_);
					get_grade_id.prepare("SELECT grade_id FROM grade WHERE grade_name = ?");
					get_grade_id.bind(0, grade_name.c_str());
					nanodbc::result new_grade_result = get_grade_id.execute();

					if (!new_grade_result.next()) {
						throw std::runtime_error("Failed to retrieve new grade ID");
					}
					grade_id = new_grade_result.get<int>(0);
				}

				// Update student's grade
				nanodbc::statement update_grade(conn_);
				update_grade.prepare("UPDATE students SET grade_id = ? WHERE user_id = ?");
				update_grade.bind(0, &grade_id);
				update_grade.bind(1, &user_id);
				update_grade.execute();
			}

			// 5. Handle class if provided
			if (!class_name.empty()) {
				int class_id = 0;
				// Try to find existing class
				nanodbc::statement find_class(conn_);
				find_class.prepare("SELECT class_id FROM classes WHERE name = ?");
				find_class.bind(0, class_name.c_str());
				nanodbc::result class_result = find_class.execute();

				if (class_result.next()) {
					class_id = class_result.get<int>(0);
				}
				else {
					// Insert new class
					nanodbc::statement insert_class(conn_);
					insert_class.prepare("INSERT INTO classes (name) VALUES (?)");
					insert_class.bind(0, class_name.c_str());
					insert_class.execute();

					// Get the new class_id
					nanodbc::statement get_class_id(conn_);
					get_class_id.prepare("SELECT class_id FROM classes WHERE name = ?");
					get_class_id.bind(0, class_name.c_str());
					nanodbc::result new_class_result = get_class_id.execute();

					if (!new_class_result.next()) {
						throw std::runtime_error("Failed to retrieve new class ID");
					}
					class_id = new_class_result.get<int>(0);
				}

				// Update student's class
				nanodbc::statement update_class(conn_);
				update_class.prepare("UPDATE students SET class_id = ? WHERE user_id = ?");
				update_class.bind(0, &class_id);
				update_class.bind(1, &user_id);
				update_class.execute();
			}

			// Commit transaction if everything succeeded
			transaction.commit();
		}
		catch (const std::exception& e) {
			throw std::runtime_error("Edit student error: " + std::string(e.what()));
		}
	}
private:
	nanodbc::connection conn_;
};

PYBIND11_MODULE(my_module, m) {
	py::class_<OracleConnector>(m, "OracleConnector")
		.def(py::init<const std::string&>())
		.def("execute_query", &OracleConnector::execute_query)
		.def("insert", &OracleConnector::insert)
		.def("get_version", &OracleConnector::get_version)
		.def("validate_credentials", &OracleConnector::validateCredentials)
		.def("count_students", &OracleConnector::count_students)
		.def("count_teachers", &OracleConnector::count_teachers)
		.def("count_classes", &OracleConnector::count_classes)
		.def("add_student", &OracleConnector::add_student)
		.def("remove_student", &OracleConnector::remove_student)
		.def("add_teacher", &OracleConnector::add_teacher)
		.def("remove_teacher", &OracleConnector::remove_teacher)
		.def("get_teachers", &OracleConnector::get_teachers)
		.def("get_students", &OracleConnector::get_students)
		.def("edit_teacher", &OracleConnector::edit_teacher)
		.def("edit_student", &OracleConnector::edit_student);



		

	// Helper function with default connection
	m.def("connect", []() {
		return OracleConnector(
			"Driver={Oracle in OraDB21Home1};"
			"DBQ=//localhost:1521/XE;"
			"Uid=c##app_user;"
			"Pwd=123;"
		);
	}, "Create a connection with default parameters");
}
//sqlplus c##app_user/123@//localhost:1521/XE