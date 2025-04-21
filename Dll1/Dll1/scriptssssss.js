// Navigation
document.querySelectorAll('.nav-item').forEach(item => {
    item.addEventListener('click', function(e) {
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
document.getElementById('themeToggle').addEventListener('change', function() {
    document.body.classList.toggle('dark-mode');
});

// Charts
const gradeCtx = document.getElementById('gradeChart').getContext('2d');
const attendanceCtx = document.getElementById('attendanceChart').getContext('2d');

new Chart(gradeCtx, {
    type: 'bar',
    data: {
        labels: ['A', 'B', 'C', 'D', 'F'],
        datasets: [{
            label: 'Number of Students',
            data: [30, 45, 35, 25, 15],
            backgroundColor: '#e74c3c'
        }]
    },
    options: {
        responsive: true,
        scales: {
            y: {
                beginAtZero: true
            }
        }
    }
});

new Chart(attendanceCtx, {
    type: 'line',
    data: {
        labels: ['Mon', 'Tue', 'Wed', 'Thu', 'Fri'],
        datasets: [{
            label: 'Attendance Rate',
            data: [95, 88, 92, 85, 90],
            borderColor: '#e74c3c',
            tension: 0.1
        }]
    },
    options: {
        responsive: true,
        scales: {
            y: {
                beginAtZero: true,
                max: 100
            }
        }
    }
});

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

function showGradebookStudents(className) {
    const studentContainer = document.querySelector('#gradebook-content .student-container');
    if (!studentContainer) return;

    studentContainer.innerHTML = '';
    
    const students = gradebookStudents[className];
    if (students) {
        students.forEach(student => {
            const studentDiv = document.createElement('div');
            studentDiv.className = 'student-item';
            studentDiv.textContent = student;
            studentDiv.onclick = () => openGradeModal(student);
            studentContainer.appendChild(studentDiv);
        });
    }
}

// Initialize gradebook class selection
document.addEventListener('DOMContentLoaded', function() {
    const classItems = document.querySelectorAll('.gradebook-class-item');
    classItems.forEach(item => {
        item.addEventListener('click', function() {
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
function openGradeModal(studentName) {
    const modal = document.getElementById('gradeModal');
    document.getElementById('studentName').textContent = studentName;
    modal.style.display = 'block';
}

// Close modal when clicking X
document.querySelector('.close').addEventListener('click', function() {
    document.getElementById('gradeModal').style.display = 'none';
});

// Close modal when clicking outside
window.addEventListener('click', function(event) {
    const modal = document.getElementById('gradeModal');
    if (event.target === modal) {
        modal.style.display = 'none';
    }
});

// Handle grade form submission
document.getElementById('gradeForm').addEventListener('submit', function(e) {
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
function updateRosterStudents(gradeId) {
    const studentContainer = document.getElementById('rosterStudentList');
    if (!studentContainer) return;

    studentContainer.innerHTML = '';
    
    const students = rosterStudents[gradeId];
    if (students) {
        students.forEach(student => {
            const studentDiv = document.createElement('div');
            studentDiv.className = 'student-item';
            studentDiv.textContent = student;
            studentContainer.appendChild(studentDiv);
        });
    }
}

// Add event listener for class selection
document.addEventListener('DOMContentLoaded', function() {
    const classSelect = document.getElementById('classSelect');
    if (classSelect) {
        classSelect.addEventListener('change', function() {
            updateRosterStudents(this.value);
        });
        
        // Show initial students
        updateRosterStudents(classSelect.value);
    }
});