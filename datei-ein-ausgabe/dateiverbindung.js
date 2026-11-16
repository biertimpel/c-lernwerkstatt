(() => {
  const steps = [
    ['Vor dem Öffnen', 'datei enthält NULL. Es ist noch keine Dateiverbindung geöffnet.', 'FILE *datei = NULL;', 0],
    ['Öffnen erfolgreich', 'fopen hat eine Dateiverbindung zum Schreiben geöffnet. datei enthält die Adresse des FILE-Verwaltungsobjekts. Die vorhandene notiz.txt wurde durch den Modus "w" geleert.', 'datei = fopen("notiz.txt", "w");', 1],
    ['Text ausgeben', 'fprintf übernimmt die Ausgabe. Zeichen können zunächst im Puffer im Arbeitsspeicher liegen, bevor sie in die Datei geschrieben werden. Prüfen Sie den Rückgabewert auf einen Schreibfehler.', 'int geschrieben = fprintf(datei, "Messwert: %d\\n", 7);', 2],
    ['Schließen erfolgreich', 'fclose hat auch die noch im Ausgabepuffer liegenden Zeichen geschrieben und die Dateiverbindung geschlossen. Die Datei bleibt erhalten. Für diese Verbindung darf datei nicht erneut verwendet werden.', 'int geschlossen = fclose(datei);', 3]
  ];
  let current = 0;
  const el = id => document.getElementById(id);
  function render() {
    const [phase, text, code, number] = steps[current];
    el('file-phase').textContent = phase;
    el('file-explanation').textContent = text;
    el('file-code').textContent = code;
    el('file-step').textContent = `Schritt ${current + 1} von ${steps.length}`;
    el('file-prev').disabled = current === 0;
    el('file-next').disabled = current === 3;
    el('pointer-text').textContent = number === 0 ? 'NULL' : number === 3 ? 'nicht mehr verwenden' : 'Adresse des FILE-Objekts';
    el('disk-text').textContent = ['Datei auf Datenträger', 'leer', 'ggf. noch gepuffert', 'Messwert: 7'][number];
    el('pointer-link').style.visibility = number === 1 || number === 2 ? 'visible' : 'hidden';
    el('stream-link').style.visibility = number === 1 || number === 2 ? 'visible' : 'hidden';
    el('stream-box').classList.toggle('active', number === 1 || number === 2);
    el('file-desc').textContent = phase + '. ' + text;
  }
  el('file-prev').onclick = () => { current = Math.max(0, current - 1); render(); };
  el('file-next').onclick = () => { current = Math.min(3, current + 1); render(); };
  el('file-reset').onclick = () => { current = 0; render(); };
  render();
})();
