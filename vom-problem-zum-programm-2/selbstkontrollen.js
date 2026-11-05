'use strict';

document.querySelectorAll('.self-check:not(.test-cases)').forEach(form => {
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

document.querySelectorAll('.test-cases').forEach(form => {
  const cases = Array.from(form.querySelectorAll('.test-case'));
  const places = 24;
  const checks = {
    free: count => count < places,
    full: count => count === places,
    overflow: count => count > places
  };
  form.querySelector('button').hidden = false;

  form.addEventListener('submit', event => {
    event.preventDefault();
    cases.forEach(testCase => {
      const input = testCase.querySelector('input');
      const feedback = testCase.querySelector('.feedback');
      const value = input.value.trim();
      const count = Number(value);
      const valid = /^\d+$/.test(value) && Number.isSafeInteger(count);
      input.setAttribute('aria-invalid', String(!valid));
      if (!valid) {
        feedback.dataset.state = 'notice';
        feedback.textContent = value === ''
          ? 'Tragen Sie zunächst eine Anzahl Teilnehmender ein.'
          : /^\d+$/.test(value)
            ? 'Diese Zahl ist für die Browser-Auswertung zu groß. Wählen Sie eine kleinere ganze Zahl.'
            : 'Geben Sie eine nichtnegative ganze Zahl ein, zum Beispiel 0 oder 19.';
        return;
      }

      const correct = checks[testCase.dataset.case](count);
      const situation = count < places
        ? (places - count === 1 ? 'bleibt 1 Platz frei' : 'bleiben ' + (places - count) + ' Plätze frei')
        : count === places
          ? 'ist der Raum genau voll'
          : (count - places === 1 ? 'fehlt 1 Platz' : 'fehlen ' + (count - places) + ' Plätze');
      const hints = {
        free: 'Wählen Sie einen Wert, bei dem Plätze frei bleiben.',
        full: 'Wählen Sie einen Wert, bei dem der Raum genau voll ist.',
        overflow: 'Wählen Sie einen Wert, bei dem Plätze fehlen.'
      };
      feedback.dataset.state = correct ? 'success' : 'notice';
      feedback.textContent = (correct ? '✓ Richtig. ' : 'Noch nicht ganz. ')
        + 'Bei ' + count + ' Teilnehmenden ' + situation + '.'
        + (correct ? '' : ' ' + hints[testCase.dataset.case]);
    });
  });

  cases.forEach(testCase => {
    const input = testCase.querySelector('input');
    const feedback = testCase.querySelector('.feedback');
    input.addEventListener('input', () => {
      feedback.textContent = '';
      delete feedback.dataset.state;
      input.removeAttribute('aria-invalid');
    });
  });
});
