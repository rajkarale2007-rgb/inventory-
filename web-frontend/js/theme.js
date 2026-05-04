// Theme Management Logic
export function initTheme() {
    const themeToggleBtn = document.getElementById('themeToggle');
    let isDark = localStorage.getItem('theme') === 'dark';

    function applyTheme() {
        if (isDark) {
            document.documentElement.setAttribute('data-theme', 'dark');
            themeToggleBtn.textContent = '☀️';
        } else {
            document.documentElement.removeAttribute('data-theme');
            themeToggleBtn.textContent = '🌙';
        }
    }

    themeToggleBtn.addEventListener('click', () => {
        isDark = !isDark;
        localStorage.setItem('theme', isDark ? 'dark' : 'light');
        applyTheme();
    });

    // Initial theme setup
    applyTheme();
}
