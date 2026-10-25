'use strict';

document.querySelectorAll('.self-check').forEach(form => {
  const choices = Array.from(form.querySelectorAll('input[type="checkbox"]'));
  const feedback = form.querySelector('.feedback');
  form.querySelector('button').hidden = false;

  form.addEventListener('submit', event => {
    event.preventDefault();
    if (!choices.some(choice => choice.checked)) {
      feedback.dataset.state = 'notice';
      feedback.textContent = 'Wählen Sie zunächst mindestens eine Antwort aus.';
      return;
    }
    const correct = choices.every(choice => choice.checked === (choice.dataset.correct === 'true'));
    feedback.dataset.state = correct ? 'success' : 'notice';
    feedback.textContent = correct
      ? '✓ Richtig. Sie haben alle richtigen und keine falschen Antworten ausgewählt. Die Erklärung finden Sie unter der Frage.'
      : 'Noch nicht ganz. ' + form.dataset.hint;
  });

  form.addEventListener('change', () => {
    feedback.textContent = '';
    delete feedback.dataset.state;
  });
});
