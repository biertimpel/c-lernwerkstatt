(() => {
const steps=[
['Beim Bauen · getrennte Objektdateien','main.o enthält einen Aufruf von addiere. Der Verweis ist noch nicht mit der Implementierung in calc.o verbunden.',false,'none',-1],
['Beim Bauen · der Linker verbindet','Der Linker ordnet die Codebereiche an und verbindet den Aufruf mit der Implementierung. Er kopiert den Funktionskörper dabei nicht an die Aufrufstelle.',true,'both',-1],
['Fertig gebaut · noch nicht gestartet','Das Programm enthält beide Codebereiche und den verbundenen Aufruf. Bis hierher wurde die Rechenfunktion noch nicht ausgeführt.',true,'none',-1],
['Beim Ausführen · Aufruf in main','Der Ablauf erreicht addiere(4, 7). Die Argumentwerte werden übergeben. Die Fortsetzung in main wird für die Rückkehr festgehalten.',true,'main',0],
['Beim Ausführen · in addiere','Der Ablauf folgt dem Aufruf zur Funktion. Mit a = 4 und b = 7 wird der Ausdruck a + b ausgewertet.',true,'add',4],
['Beim Ausführen · Rückkehr mit 11','return liefert 11 zurück. Der Aufrufer setzt den Ablauf fort und weist das Ergebnis summe zu.',true,'return',0],
['Beim Ausführen · weiter in main','Die anschließende printf-Anweisung gibt das Ergebnis aus. Der Funktionscode blieb ein eigener Codebereich im selben geladenen Programm.',true,'main',1]
];
const lines=['int summe = addiere(4, 7);','printf("Summe: %d\\n", summe);','','int addiere(int a, int b) {','    return a + b;','}'];
let index=0;const el=id=>document.getElementById(id);
function render(){
 const [phase,copy,linked,active,line]=steps[index];
 el('phase').textContent=phase;el('explanation').textContent=copy;
 el('step-count').textContent=`Schritt ${index+1} von ${steps.length}`;
 el('prev').disabled=index===0;el('next').disabled=index===steps.length-1;
 el('container-label').textContent=linked?'Ein Programm mit getrennten Codebereichen':'Getrennte Objektdateien';
 el('main-label').textContent=linked?'Codebereich main':'main.o';el('add-label').textContent=linked?'Codebereich addiere':'calc.o';
 el('target-label').textContent=linked?'Ziel: Codebereich addiere':'Ziel: noch nicht verbunden';
 el('result-label').textContent=index>=5?'summe: 11':'summe: noch kein Ergebnis';
 el('args-label').textContent=index>=4?'a = 4, b = 7':'';
 const showCall=index===1||index===3||index===4;
 el('call-arrow').style.visibility=showCall?'visible':'hidden';el('call-label').style.visibility=showCall?'visible':'hidden';
 el('call-label').textContent=index===1?'Verweis':'Aufruf';
 el('return-arrow').style.visibility=index===5?'visible':'hidden';el('return-label').style.visibility=index===5?'visible':'hidden';
 el('main-box').classList.toggle('active',active==='main'||active==='return'||active==='both');
 el('add-box').classList.toggle('active',active==='add'||active==='both');
 el('active-code').replaceChildren(...lines.map((text,i)=>{let s=document.createElement('span');s.className='code-line'+(i===line?' active-code':'');s.textContent=text||' ';return s;}));
 el('diagram-description').textContent=phase+'. '+copy;
}
el('prev').addEventListener('click',()=>{index=Math.max(0,index-1);render()});
el('next').addEventListener('click',()=>{index=Math.min(steps.length-1,index+1);render()});
el('reset').addEventListener('click',()=>{index=0;render()});render();
})();