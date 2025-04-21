const currentUser = JSON.parse(localStorage.getItem('currentUser'));
const currentUsername = currentUser ? currentUser.username : null;
const API_BASE = 'http://localhost:5000';



// Function to fetch all grades taught by the teacher
async function fetchTeacherGrades() {
    try {
        const response = await fetch(`${API_BASE}/grades/teacher/${currentUsername}`);
        if (!response.ok) throw new Error('Failed to fetch teacher grades');
        const gradeData = await response.json();
        return Object.keys(gradeData); // Returns array like ["Grade 9", "Grade 10"]
    } catch (error) {
        console.error('Error fetching teacher grades:', error);
        return [];
    }
}

// Function to fetch students for a specific grade
async function fetchGradeStudents(grade) {
    try {
        const response = await fetch(`${API_BASE}/get-student/teacher/${currentUsername}/${encodeURIComponent(grade)}`);
        if (!response.ok) throw new Error(`Failed to fetch students for ${grade}`);
        return await response.json();
    } catch (error) {
        console.error(`Error fetching students for ${grade}:`, error);
        return [];
    }
}

// Initialize the gradebook with dynamic classes
async function initializeGradebook() {
    const classSelect = document.querySelector('#gradebook-content .class-select');
    const studentContainer = document.querySelector('#gradebook-content .student-container');
    
    if (!classSelect || !studentContainer) return;
    
    // Show loading states
    classSelect.innerHTML = '<div class="loading">Loading classes...</div>';
    studentContainer.innerHTML = '<div class="loading">Select a class to view students</div>';
    
    try {
        // Fetch all grades the teacher teaches
        const grades = await fetchTeacherGrades();
        
        // Clear and populate class select
        classSelect.innerHTML = '';
        
        if (grades.length === 0) {
            classSelect.innerHTML = '<div class="no-classes">No classes assigned</div>';
            return;
        }
        
        // Create grade items
        grades.forEach(grade => {
            const gradeItem = document.createElement('div');
            gradeItem.className = 'gradebook-class-item';
            gradeItem.textContent = grade;
            
            gradeItem.addEventListener('click', async () => {
                // Remove active class from all items
                document.querySelectorAll('.gradebook-class-item').forEach(item => {
                    item.classList.remove('active');
                });
                
                // Add active class to clicked item
                gradeItem.classList.add('active');
                
                // Load students for this grade
                await loadStudentsForGrade(grade);
            });
            
            classSelect.appendChild(gradeItem);
        });
        
        // Activate the first grade by default
        const firstGrade = classSelect.querySelector('.gradebook-class-item');
        if (firstGrade) {
            firstGrade.classList.add('active');
            await loadStudentsForGrade(firstGrade.textContent);
        }
    } catch (error) {
        classSelect.innerHTML = '<div class="error">Error loading classes</div>';
        console.error('Error initializing gradebook:', error);
    }
}

// Load students for a specific grade
async function loadStudentsForGrade(grade) {
    const studentContainer = document.querySelector('#gradebook-content .student-container');
    if (!studentContainer) return;
    
    studentContainer.innerHTML = '<div class="loading">Loading students...</div>';
    
    try {
        const students = await fetchGradeStudents(grade);
        
        studentContainer.innerHTML = '';
        
        if (students.length === 0) {
            studentContainer.innerHTML = '<div class="no-students">No students in this class</div>';
            return;
        }
        
        // Group students by class name
        const classes = {};
        students.forEach(student => {
            if (!classes[student.class_name]) {
                classes[student.class_name] = [];
            }
            classes[student.class_name].push(student);
        });
        
        // Create sections for each class
        Object.entries(classes).forEach(([className, classStudents]) => {
            // Add class header
            const classHeader = document.createElement('h4');
            classHeader.className = 'class-header';
            classHeader.textContent = className;
            studentContainer.appendChild(classHeader);
            
            // Add students for this class
            classStudents.forEach(student => {
                const studentDiv = document.createElement('div');
                studentDiv.className = 'student-item';
                studentDiv.innerHTML = `
                    <span class="student-name">${student.username}</span>
                `;
                studentContainer.appendChild(studentDiv);
            });
        });
    } catch (error) {
        studentContainer.innerHTML = '<div class="error">Error loading students</div>';
        console.error('Error loading students:', error);
    }
}

async function fetchAllStudents() {
    try {
        const response = await fetch(`${API_BASE}/users/list/students`);
        if (!response.ok) {
            throw new Error('Failed to fetch students');
        }
        const data = await response.json();
        return data.map(student => student.username); // Extract only usernames
    } catch (error) {
        console.error('Error fetching students:', error);
        return []; // Return empty array if there's an error
    }
}


async function fetchGradeDistribution(username) {
    try {
        const response = await fetch(`${API_BASE}/grades/teacher/${username}`);
        if (!response.ok) {
            throw new Error('Failed to fetch grade distribution');
        }
        return await response.json();
    } catch (error) {
        console.error('Error fetching grade distribution:', error);
        return {}; // Return empty object if there's an error
    }
}

// Add this function to fetch student count
async function fetchStudentCount() {
    try {
        const response = await fetch(`${API_BASE}/users/count/student`);
        if (!response.ok) {
            throw new Error('Failed to fetch student count');
        }
        const data = await response.json();
        return data.count; // Assuming the API returns { count: number }
    } catch (error) {
        console.error('Error fetching student count:', error);
        return 0; // Return 0 if there's an error
    }
}

// Update the card display function
async function updateDashboardCards() {
    const studentCount = await fetchStudentCount();
    const totalStudentsCard = document.querySelector('.card:nth-child(1) .card-value');
    if (totalStudentsCard) {
        totalStudentsCard.textContent = studentCount;
    }
}


// Navigation
document.querySelectorAll('.nav-item').forEach(item => {
    item.addEventListener('click', function (e) {
        e.preventDefault();

        // Remove active class from all items
        document.querySelectorAll('.nav-item').forEach(i => i.classList.remove('active'));

        // Add active class to clicked item
        this.classList.add('active');

        // Hide all content sections
        document.querySelectorAll('.dashboard-content').forEach(content => {
            content.style.display = 'none';
        });

        // Show selected content
        const contentId = this.getAttribute('data-content');
        document.getElementById(`${contentId}-content`).style.display = 'block';
    });
});

// Theme Toggle
document.getElementById('themeToggle').addEventListener('change', function () {
    document.body.classList.toggle('dark-mode');
});

// Charts
const gradeCtx = document.getElementById('gradeChart').getContext('2d');

async function initializeGradeChart() {
    const gradeData = await fetchGradeDistribution(currentUsername);
    
    // Convert the grade data to chart format
    const labels = Object.keys(gradeData);
    const dataValues = Object.values(gradeData);
    
    const gradeCtx = document.getElementById('gradeChart').getContext('2d');
    
    new Chart(gradeCtx, {
        type: 'bar',
        data: {
            labels: labels,
            datasets: [{
                label: 'Number of Classes',
                data: dataValues,
                backgroundColor: '#e74c3c'
            }]
        },
        options: {
            responsive: true,
            scales: {
                y: {
                    beginAtZero: true,
                    title: {
                        display: true,
                        text: 'Number of Classes'
                    }
                },
                x: {
                    title: {
                        display: true,
                        text: 'Grade Level'
                    }
                }
            },
            plugins: {
                title: {
                    display: true,
                    text: 'Classes by Grade Level'
                }
            }
        }
    });
}
// Gradebook functionality
const gradebookStudents = {
    'Grade 1': [
        "Ahmed Mohamed",
        "Sara Ahmed",
        "Mohamed Ali",
        "Nour Hassan",
        "Youssef Ibrahim"
    ],
    'Grade 2': [
        "Mariam Ahmed",
        "Omar Khaled",
        "Layla Mahmoud",
        "Ziad Hassan",
        "Rana Mohamed"
    ],
    'Grade 3': [
        "Karim Ali",
        "Hana Ahmed",
        "Ali Hassan",
        "Salma Omar",
        "Amr Mahmoud"
    ]
};

async function fetchGradebookStudents(grade) {
    try {
        const response = await fetch(`${API_BASE}/get-student/teacher/${currentUsername}/${grade}`);
        if (!response.ok) {
            throw new Error(`Failed to fetch students for grade ${grade}`);
        }
        return await response.json();
    } catch (error) {
        console.error('Error fetching gradebook students:', error);
        return [];
    }
}

async function showGradebookStudents(gradeName) {
    const studentContainer = document.querySelector('#gradebook-content .student-container');
    if (!studentContainer) return;

    studentContainer.innerHTML = '<div class="loading">Loading students...</div>';

    try {
        const students = await fetchGradebookStudents(gradeName);
        
        studentContainer.innerHTML = '';
        
        if (students.length === 0) {
            studentContainer.innerHTML = '<div class="no-students">No students found in this class</div>';
            return;
        }

        // Group students by class name
        const studentsByClass = {};
        students.forEach(student => {
            if (!studentsByClass[student.class_name]) {
                studentsByClass[student.class_name] = [];
            }
            studentsByClass[student.class_name].push(student);
        });

        // Create sections for each class
        for (const [className, classStudents] of Object.entries(studentsByClass)) {
            const classHeader = document.createElement('div');
            classHeader.className = 'class-header';
            classHeader.textContent = className;
            studentContainer.appendChild(classHeader);

            classStudents.forEach(student => {
                const studentDiv = document.createElement('div');
                studentDiv.className = 'student-item';
                
            // Display student username
                studentDiv.innerHTML = `
                    <span class="student-name">${student.username}</span>
                    <button class="btn-grade" data-username="${student.username}" data-class="${className}">
                        Add Grade
                    </button>
                `;
                
                        // Add click handler for the grade button
                studentDiv.querySelector('.btn-grade').addEventListener('click', (e) => {
                    e.stopPropagation();
                    openGradeModal(student.username, className);
                    });
                
                studentContainer.appendChild(studentDiv);
                    });
                    }
                    } catch (error) {
        studentContainer.innerHTML = '<div class="error">Error loading students</div>';
        console.error('Error showing gradebook students:', error);
                    }
              }

document.addEventListener('DOMContentLoaded', function () {
    const classItems = document.querySelectorAll('.gradebook-class-item');
    classItems.forEach(item => {
        item.addEventListener('click', function () {
            classItems.forEach(i => i.classList.remove('active'));
            this.classList.add('active');
            showGradebookStudents(this.textContent.trim());
        });
    });

    // Show initial students
    const firstClass = document.querySelector('.gradebook-class-item');
    if (firstClass) {
        showGradebookStudents(firstClass.textContent.trim());
    }
});

// Modal functionality
function openGradeModal(studentName, className = '') {
    const modal = document.getElementById('gradeModal');
    document.getElementById('studentName').textContent = studentName;
    
    // Add class name to modal if available
    const classInfo = document.getElementById('classInfo');
    if (className && classInfo) {
        classInfo.textContent = `Class: ${className}`;
        classInfo.style.display = 'block';
    } else if (classInfo) {
        classInfo.style.display = 'none';
    }
    
    modal.style.display = 'block';
}
    // Close modal when clicking X
document.querySelector('.close').addEventListener('click', function () {
    document.getElementById('gradeModal').style.display = 'none';
});

// Close modal when clicking outside
window.addEventListener('click', function (event) {
    const modal = document.getElementById('gradeModal');
    if (event.target === modal) {
        modal.style.display = 'none';
    }
});

// Handle grade form submission
document.getElementById('gradeForm').addEventListener('submit', function (e) {
    e.preventDefault();
    const studentName = document.getElementById('studentName').textContent;
    const examType = document.getElementById('examType').value;
    const grade = document.getElementById('grade').value;

    console.log('Saving grade:', {
        student: studentName,
        examType: examType,
        grade: grade
    });

    // Clear form and close modal
    this.reset();
    document.getElementById('gradeModal').style.display = 'none';
});

// Add this to your existing JavaScript
const rosterStudents = {
    'grade1': [
        "Ahmed Mohamed",
        "Sara Ahmed",
        "Mohamed Ali",
        "Nour Hassan",
        "Youssef Ibrahim"
    ],
    'grade2': [
        "Mariam Ahmed",
        "Omar Khaled",
        "Layla Mahmoud",
        "Ziad Hassan",
        "Rana Mohamed"
    ],
    'grade3': [
        "Karim Ali",
        "Hana Ahmed",
        "Ali Hassan",
        "Salma Omar",
        "Amr Mahmoud"
    ],
    'grade4': [
        "Yasmin Khaled",
        "Adam Hassan",
        "Malak Ibrahim",
        "Hamza Ahmed",
        "Nada Ali"
    ]
};

// Function to update student list in roster
// Replace the rosterStudents object with this function
async function updateRosterStudents(gradeFilter = null) {
    const studentContainer = document.getElementById('rosterStudentList');
    if (!studentContainer) return;

    studentContainer.innerHTML = '<div class="loading">Loading students...</div>';

    try {
        let students = [];
        
        if (gradeFilter && gradeFilter !== 'all') {
            // Use the filtered endpoint when a grade is selected
            const response = await fetch(`${API_BASE}/student/teacher/${currentUsername}/${gradeFilter}`);
            if (!response.ok) {
                throw new Error(`Failed to fetch students for grade ${gradeFilter}`);
            }
            students = await response.json();
        } else {
            // Use the full list endpoint when "All Students" is selected
            const response = await fetch(`${API_BASE}/users/list/students`);
            if (!response.ok) {
                throw new Error('Failed to fetch all students');
            }
            const data = await response.json();
            students = data.map(student => student.username);
        }

        studentContainer.innerHTML = '';
        
        if (students.length === 0) {
            studentContainer.innerHTML = '<div class="no-students">No students found</div>';
            return;
        }

        students.forEach(username => {
            const studentDiv = document.createElement('div');
            studentDiv.className = 'student-item';
            studentDiv.textContent = username;
            studentContainer.appendChild(studentDiv);
        });
    } catch (error) {
        studentContainer.innerHTML = '<div class="error">Error loading students</div>';
        console.error('Error updating roster:', error);
    }
}
    
function displayCurrentUser() {
    if (!currentUsername) {
        console.error("No user logged in - redirecting to login");
        window.location.href = 'login.html';
        return false;
    }
    // Update both the welcome message and the user name in the header
    document.querySelector('.user-info').textContent = `Welcome, ${currentUsername}`;
    return true;
}


document.querySelector('.logout-btn')?.addEventListener('click', function() {
    if (confirm("Are you sure you want to logout?")) {
        localStorage.removeItem('currentUser');
        alert("You have been logged out");
        window.location.href = 'login.html';
    }
});


async function initializeClassDropdown() {
    try {
        const response = await fetch(`${API_BASE}/grades/teacher/${currentUsername}`);
        if (!response.ok) {
            throw new Error('Failed to fetch grade data');
        }
        const gradeData = await response.json();
        
        // Get the grade names from the keys of the response object
        const gradeNames = Object.keys(gradeData);
        
        const classSelect = document.getElementById('classSelect');
        if (!classSelect) return;
        
        // Clear existing options
        classSelect.innerHTML = '';
        
        // Add "All Students" option
        const allOption = document.createElement('option');
        allOption.value = 'all';
        allOption.textContent = 'All Students';
        classSelect.appendChild(allOption);
        
        // Add grade options
        gradeNames.forEach(grade => {
            const option = document.createElement('option');
            option.value = grade;
            option.textContent = grade;
            classSelect.appendChild(option);
        });
        
        // Add event listener
        classSelect.addEventListener('change', function() {
            updateRosterStudents(this.value === 'all' ? null : this.value);
        });
        
    } catch (error) {
        console.error('Error initializing class dropdown:', error);
        // Fallback to basic options if there's an error
        const classSelect = document.getElementById('classSelect');
        if (classSelect) {
            classSelect.innerHTML = `
                <option value="all">All Students</option>
                <option value="Grade 9">Grade 9</option>
                <option value="Grade 10">Grade 10</option>
                <option value="Grade 11">Grade 11</option>
                <option value="Grade 12">Grade 12</option>
            `;
            classSelect.addEventListener('change', function() {
                updateRosterStudents(this.value === 'all' ? null : this.value);
            });
        }
    }
}
document.addEventListener('DOMContentLoaded', async function () {
    // Check and display current user
    if (!displayCurrentUser()) {
        return;
    }

    // Initialize components
    await updateDashboardCards();
    await initializeGradeChart();
    await initializeClassDropdown();
    await updateRosterStudents();
    await initializeGradebook(); // Initialize dynamic gradebook clas
    // Initialize gradebook class selection
    const classItems = document.querySelectorAll('.gradebook-class-item');
    classItems.forEach(item => {
        item.addEventListener('click', function() {
            classItems.forEach(i => i.classList.remove('active'));
            this.classList.add('active');
            const gradeName = this.textContent.trim();
            showGradebookStudents(gradeName);
        });
    });

    // Show initial students (first class by default)
    const firstClass = document.querySelector('.gradebook-class-item');
    if (firstClass) {
        firstClass.classList.add('active');
        showGradebookStudents(firstClass.textContent.trim());
    }
});