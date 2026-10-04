/**
 * Chart.js Visualization Module
 * Renders interactive attendance doughnut charts and subject performance bar charts.
 */

function renderAttendanceChart(canvasId, presentCount, absentCount) {
    const ctx = document.getElementById(canvasId).getContext('2d');
    new Chart(ctx, {
        type: 'doughnut',
        data: {
            labels: ['Present Sessions', 'Absent Sessions'],
            datasets: [{
                data: [presentCount, absentCount],
                backgroundColor: ['#198754', '#dc3545'],
                borderWidth: 2,
                borderColor: '#ffffff'
            }]
        },
        options: {
            responsive: true,
            maintainAspectRatio: false,
            plugins: {
                legend: {
                    position: 'bottom',
                    labels: {
                        font: { size: 14, family: 'Segoe UI' }
                    }
                }
            }
        }
    });
}

function renderMarksChart(canvasId, subjectLabels, internal1Data, internal2Data, assignmentData, finalData) {
    const ctx = document.getElementById(canvasId).getContext('2d');
    new Chart(ctx, {
        type: 'bar',
        data: {
            labels: subjectLabels,
            datasets: [
                {
                    label: 'Internal 1 (Max 20)',
                    data: internal1Data,
                    backgroundColor: '#0d6efd'
                },
                {
                    label: 'Internal 2 (Max 20)',
                    data: internal2Data,
                    backgroundColor: '#6f42c1'
                },
                {
                    label: 'Assignment (Max 10)',
                    data: assignmentData,
                    backgroundColor: '#0dcaf0'
                },
                {
                    label: 'Final Exam (Max 50)',
                    data: finalData,
                    backgroundColor: '#198754'
                }
            ]
        },
        options: {
            responsive: true,
            maintainAspectRatio: false,
            scales: {
                x: { stacked: false },
                y: { beginAtZero: true, max: 50 }
            },
            plugins: {
                legend: { position: 'bottom' }
            }
        }
    });
}
