const darkModeToggle = document.querySelector('.dark-mode-toggle');
const body = document.body;
const API_BASE = 'http://localhost:5000';

// Data storage
let students = [];
let staff = [];

// Dashboard elements
const studentsElement = document.getElementById('students');
const teachersElement = document.getElementById('teachers');
const classesElement = document.getElementById('classes');
const studentsCountElement = document.getElementById('students-count');
const staffCountElement = document.getElementById('staff-count');





async function editStudentData(username, password, grade_name, class_name) {
    try {
        const response = await makeApiRequest(
            'PUT', 
            `/student/edit/${encodeURIComponent(username)}/${encodeURIComponent(password)}/${encodeURIComponent(grade_name)}/${encodeURIComponent(class_name)}`
        );
        
        if (response?.Message === "Student updated successfully" || response?.Message === "done") {
            await fetchStudents(); // Refresh student list
            alert(`Student ${username} updated successfully!`);
            return true;
        }
    throw new Error('Failed to update student');
} catch (error) {
    alert(`Error updating student: ${error.message}`);
    return false;
}
}

// Display current user
function displayCurrentUser() {
    const userData = JSON.parse(localStorage.getItem('currentUser'));
    if (userData && userData.username) {
        document.querySelector('.user-info').textContent = `Welcome, ${userData.username}`;
    }
}

// Add this function to handle teacher editing
async function editTeacherData(username, password, grade_name, class_name, subject) {
    try {
        const response = await makeApiRequest(
            'PUT', 
            `/teacher/edit/${encodeURIComponent(username)}/${encodeURIComponent(password)}/${encodeURIComponent(grade_name)}/${encodeURIComponent(class_name)}/${encodeURIComponent(subject)}`
        );
        
        if (response?.Message === "Teacher updated successfully" || response?.Message === "done") {
            await fetchStaff(); // Refresh staff list
            alert(`Staff member ${username} updated successfully!`);
            return true;
        }
    throw new Error('Failed to update staff member');
} catch (error) {
    alert(`Error updating staff: ${error.message}`);
    return false;
}
}

// Tables
const studentsTable = document.getElementById('students-table').querySelector('tbody');
const staffTable = document.getElementById('staff-table').querySelector('tbody');
        // Initialize the pie chart
// Initialize the pie chart with dynamic data
document.addEventListener('DOMContentLoaded', async function() {
    const ctx = document.getElementById('studentChart').getContext('2d');
    
    // Fetch grade distribution data
    const gradeData = await fetchGradeDistribution();
    
    // Default data in case API fails
    let labels = ['Grade 9', 'Grade 10', 'Grade 11', 'Grade 12', 'Grade 13'];
    let dataValues = [120, 150, 180, 200, 80];
    
    if (gradeData) {
        // Extract labels and values from the API response
        labels = Object.keys(gradeData);
        dataValues = Object.values(gradeData);
    }
    
    const studentChart = new Chart(ctx, {
        type: 'pie',
        data: {
            labels: labels,
            datasets: [{
                data: dataValues,
                backgroundColor: [
                    '#9b59b6',
                    '#3498db',
                    '#2ecc71',
                    '#f1c40f',
                    '#e74c3c',
                    '#1abc9c', // Additional colors if there are more grades
                    '#d35400'
                ],
                borderWidth: 1
            }]
        },
        options: {
            responsive: true,
            maintainAspectRatio: false,
            plugins: {
                legend: {
                    position: 'right',
                    labels: {
                        color: function(context) {
                            // Dynamically set label color based on dark mode
                            return document.body.classList.contains('dark-mode') ? '#e0e0e0' : '#333';
                        }
                    }
                },
                tooltip: {
                    callbacks: {
                        label: function(context) {
                            const label = context.label || '';
                            const value = context.raw || 0;
                            const total = context.dataset.data.reduce((a, b) => a + b, 0);
                            const percentage = Math.round((value / total) * 100);
                            return `${label}: ${value} (${percentage}%)`;
                        }
                    }
                }
            }
        }
    });

    // Update chart when dark mode changes
    const darkModeToggle = document.querySelector('.dark-mode-toggle');
    darkModeToggle.addEventListener('click', function() {
        setTimeout(() => {
            studentChart.update();
        }, 0);
    });
    
    // Refresh chart data periodically (every 5 minutes)
    setInterval(async () => {
        const newGradeData = await fetchGradeDistribution();
        if (newGradeData) {
            studentChart.data.labels = Object.keys(newGradeData);
            studentChart.data.datasets[0].data = Object.values(newGradeData);
            studentChart.update();
        }
    }, 300000); // 300000 ms = 5 minutes
});
// API Helper Function
async function makeApiRequest(method, endpoint, data = null) {
    const options = {
        method: method,
        headers: {
            'Content-Type': 'application/json'
        }
    };
    
    if (data) {
        options.body = JSON.stringify(data);
    }
    
    try {
        const response = await fetch(`${API_BASE}${endpoint}`, options);
        
        if (!response.ok) {
            const errorDetails = await response.text();
            throw new Error(`HTTP error! status: ${response.status}, message: ${errorDetails || 'Unknown error'}`);
        }
        
        return await response.json();
    } catch (error) {
        console.error(`API request error:`, error.message);
        throw error;
    }
}

// Fetch all students using list endpoint
async function fetchStudents() {
    try {
        const response = await makeApiRequest('GET', '/users/list/students');
        students = response.map(student => ({
            username: student.username,
            grade: student.grade_name || 'N/A',
            class: student.class_name || 'N/A',
            user_id: student.user_id
        }));
        
        updateStudentsTable();
        studentsCountElement.textContent = students.length;
        return true;
    } catch (error) {
        console.error("Error fetching students:", error);
        return false;
    }
}

async function fetchGradeDistribution() {
    try {
        const response = await makeApiRequest('GET', '/count/grades');
        return response;
    } catch (error) {
        console.error("Error fetching grade distribution:", error);
        return null;
    }
}

// Fetch all staff using list endpoint
// Fetch all staff using list endpoint
async function fetchStaff() {
    try {
        const response = await makeApiRequest('GET', '/users/list/teachers');
        
        staff = response.map(teacher => ({
            username: teacher.username,
            // Since there's no position field, we'll create one based on subjects
            position: teacher.subjects ? teacher.subjects.split(',')[0] + ' Teacher' : 'Teacher',
            grades: teacher.grades || 'N/A',
            classes: teacher.classes || 'N/A',
            subjects: teacher.subjects || 'General'
        }));
        
        updateStaffTable();
        staffCountElement.textContent = staff.length;
        return true;
    } catch (error) {
        console.error("Error fetching staff:", error);
        return false;
    }
}

// Count Functions
async function updateCounts() {
    try {
        const [studentsRes, teachersRes, classesRes] = await Promise.all([
            makeApiRequest('GET', '/users/count/student'),
            makeApiRequest('GET', '/users/count/teacher'),
            makeApiRequest('GET', '/users/count/class')
        ]);

        studentsElement.textContent = studentsRes?.count || 0;
        teachersElement.textContent = teachersRes?.count || 0;
        classesElement.textContent = classesRes?.count || 0;
    } catch (error) {
        console.error("Error updating counts:", error);
    }
}

async function addStudent(name, password, grade, studentClass) {
    try {
        const response = await makeApiRequest(
            'PUT', 
            `/users/add/student/${encodeURIComponent(name)}/${encodeURIComponent(password)}/${encodeURIComponent(grade)}/${encodeURIComponent(studentClass)}`
        );
        
        if (response?.Message === "Student added successfully") {
            await fetchStudents(); // Refresh student list
            alert(`Student ${name} added successfully!`);
            return true;
        } else {
            alert(`Failed to add student: ${response?.error || 'Unknown error'}`);
            return false;
        }
    } catch (error) {
        alert(`Error adding student: ${error.message}`);
        return false;
    }
}

async function deleteStudent(username) {
    try {
        if (!confirm(`Are you sure you want to delete student ${username}?`)) {
            return false;
        }
        
        const response = await makeApiRequest(
            'PUT', 
            `/users/remove/student/${encodeURIComponent(username)}`
        );
        
        if (response?.Message === "done") {
            await fetchStudents(); // Refresh student list
            alert(`Student ${username} removed successfully!`);
            return true;
        }
        throw new Error('Failed to remove student');
    } catch (error) {
        alert(`Error removing student: ${error.message}`);
        return false;
    }
}

function updateStudentsTable() {
    studentsTable.innerHTML = students.map(student => `
    <tr>
        <td>${student.username}</td>
        <td>${student.grade}</td>
        <td>${student.class}</td>
        <td class="action-cell">
        <button class="table-btn edit-btn" onclick="editStudent('${student.username}')">Edit</button>
        <button class="table-btn delete-btn" onclick="deleteStudentHandler('${student.username}')">Delete</button>
        </td>
    </tr>
    `).join('');
}

// Staff Functions
async function addStaff(name, password, grade_name, class_name, subject) {
    try {
        // Note: Adjust this endpoint based on your actual API
        // Currently using a simplified version without position/department
        const response = await makeApiRequest(
            'PUT', 
            `/users/add/teacher/${encodeURIComponent(name)}/${encodeURIComponent(password)}/${encodeURIComponent(grade_name)}/${encodeURIComponent(class_name)}/${encodeURIComponent(subject)}`
        );
        
        if (response?.Message === "Teacher added successfully") {
            await fetchStaff(); // Refresh staff list
            alert(`Staff member ${name} added successfully!`);
            return true;
        }
        throw new Error('Failed to add staff');
    } catch (error) {
        alert(`Error adding staff: ${error.message}`);
        return false;
    }
}

async function deleteStaff(username) {
    try {
        if (!confirm(`Are you sure you want to delete staff member ${username}?`)) {
            return false;
        }
        
        const response = await makeApiRequest(
            'PUT', 
            `/users/remove/teacher/${encodeURIComponent(username)}`
        );
        
        if (response?.Message === "done") {
            await fetchStaff(); // Refresh staff list
            alert(`Staff member ${username} removed successfully!`);
            return true;
        }
        throw new Error('Failed to remove staff');
    } catch (error) {
        alert(`Error removing staff: ${error.message}`);
        return false;
    }
}



function updateStaffTable() {
    staffTable.innerHTML = staff.map(staffMember => `
        <tr>
            <td>${staffMember.username}</td>
            <td>${staffMember.grades}</td>
            <td>${staffMember.classes}</td>
            <td>${staffMember.subjects}</td>
            <td class="action-cell">
                <button class="table-btn edit-btn" onclick="editStaff('${staffMember.username}')">Edit</button>
                <button class="table-btn delete-btn" onclick="deleteStaffHandler('${staffMember.username}')">Delete</button>
            </td>
        </tr>
    `).join('');
}

// UI Event Handlers
function editStudent(username) {
    const student = students.find(s => s.username === username);
    if (student) {
        document.getElementById('student-name').value = student.username;
        document.getElementById('student-password').value = ''; // Password field emptied for security
        document.getElementById('student-grade').value = student.grade;
        document.getElementById('student-class').value = student.class;
        
        // Set form to edit mode
        document.getElementById('student-form-mode').value = 'edit';
        
        // Change the button text to indicate editing
        document.querySelector('#student-form .submit-btn').textContent = 'Update Student';
        
        document.getElementById('add-student-form').style.display = 'block';
        document.getElementById('remove-student-form').style.display = 'none';
    }
}
function editStaff(username) {
    const staffMember = staff.find(s => s.username === username);
    if (staffMember) {
        document.getElementById('staff-name').value = staffMember.username;
        document.getElementById('staff-password').value = ''; // Password field emptied for security
        document.getElementById('staff-position').value = staffMember.grades || '';
        document.getElementById('staff-class').value = staffMember.classes || '';
        document.getElementById('staff-department').value = staffMember.subjects || '';
        
        // Set form to edit mode
        document.getElementById('staff-form-mode').value = 'edit';
        
        // Change the button text to indicate editing
        document.querySelector('#staff-form .submit-btn').textContent = 'Update Staff';
        
        document.getElementById('add-staff-form').style.display = 'block';
        document.getElementById('remove-staff-form').style.display = 'none';
    }
}
// Form Handlers
document.getElementById('student-form').addEventListener('submit', async function(e) {
    e.preventDefault();
    const name = document.getElementById('student-name').value;
    const password = document.getElementById('student-password').value;
    const grade = document.getElementById('student-grade').value;
    const studentClass = document.getElementById('student-class').value;
    
    // Check if we're editing or adding
    const isEditing = document.getElementById('student-form-mode').value === 'edit';
    
    let success = false;
    if (isEditing) {
        success = await editStudentData(name, password, grade, studentClass);
    } else {
        success = await addStudent(name, password, grade, studentClass);
    }
    
    if (success) {
        this.reset();
        document.getElementById('add-student-form').style.display = 'none';
        document.getElementById('student-form-mode').value = 'add'; // Reset mode
    }
});

document.getElementById('staff-form').addEventListener('submit', async function(e) {
    e.preventDefault();
    const name = document.getElementById('staff-name').value;
    const password = document.getElementById('staff-password').value;
    const position = document.getElementById('staff-position').value;
    const class_name = document.getElementById('staff-class').value;
    const department = document.getElementById('staff-department').value;
    
    // Check if we're editing or adding
    const isEditing = document.getElementById('staff-form-mode').value === 'edit';
    
    let success = false;
    if (isEditing) {
        success = await editTeacherData(name, password, position, class_name, department);
    } else {
        success = await addStaff(name, password, position, class_name, department);
    }
    
    if (success) {
        this.reset();
        document.getElementById('add-staff-form').style.display = 'none';
        document.getElementById('staff-form-mode').value = 'add'; // Reset mode
    }
});
document.getElementById('remove-staff-form').addEventListener('submit', async function(e) {
    e.preventDefault();
    const name = document.getElementById('remove-staff-name').value;
    
    if (await deleteStaff(name)) {
        this.reset();
        document.getElementById('remove-staff-form').style.display = 'none';
    }
});

// UI Event Listeners
darkModeToggle.addEventListener('click', () => {
    body.classList.toggle('dark-mode');
    localStorage.setItem('darkMode', body.classList.contains('dark-mode') ? 'enabled' : 'disabled');
});

document.querySelectorAll('.sidebar-item').forEach(item => {
    item.addEventListener('click', function() {
        document.querySelectorAll('.sidebar-item').forEach(nav => nav.classList.remove('active'));
        this.classList.add('active');
        document.querySelectorAll('.main-content').forEach(content => content.style.display = 'none');
        document.getElementById(this.getAttribute('data-page') + '-content').style.display = 'grid';
    });
});

document.querySelector('.logout-btn').addEventListener('click', function() {
    if (confirm("Are you sure you want to logout?")) {
        localStorage.removeItem('currentUser'); // Clear user data
        alert("You have been logged out");
        window.location.href = 'login.html';
    }
});
// Button Toggles
document.getElementById('add-student-btn').addEventListener('click', function() {
    document.getElementById('add-student-form').style.display = 'block';
    document.getElementById('remove-student-form').style.display = 'none';
    document.getElementById('student-form').reset();
    document.getElementById('student-form-mode').value = 'add'; // Reset mode to add
    document.querySelector('#student-form .submit-btn').textContent = 'Add Student'; // Reset button text
});
document.getElementById('remove-student-btn').addEventListener('click', function() {
    document.getElementById('remove-student-form').style.display = 'block';
    document.getElementById('add-student-form').style.display = 'none';
    document.getElementById('remove-student-form').reset();
});

document.getElementById('add-staff-btn').addEventListener('click', function() {
    document.getElementById('add-staff-form').style.display = 'block';
    document.getElementById('remove-staff-form').style.display = 'none';
    document.getElementById('staff-form').reset();
});

document.getElementById('remove-staff-btn').addEventListener('click', function() {
    document.getElementById('remove-staff-form').style.display = 'block';
    document.getElementById('add-staff-form').style.display = 'none';
    document.getElementById('remove-staff-form').reset();
});

// Initialize
if (localStorage.getItem('darkMode') === 'enabled') {
    body.classList.add('dark-mode');
}

// Make functions global
window.deleteStudentHandler = deleteStudent;
window.deleteStaffHandler = deleteStaff;

// Initial data load
async function initializeApp() {
    try {
        displayCurrentUser(); // Add this line
        await updateCounts();
        await fetchStudents();
        await fetchStaff();
    } catch (error) {
        console.error("Initialization error:", error);
        alert("Could not connect to the server. Please make sure the backend is running.");
    }
}

document.addEventListener('DOMContentLoaded', initializeApp);