import { initTheme } from './js/theme.js';
import { UIController } from './js/UIController.js';

document.addEventListener('DOMContentLoaded', () => {
    // Initialize the Theme manager
    initTheme();
    
    // Initialize the main UI Controller which wires up logic and DOM
    UIController.init();
});
