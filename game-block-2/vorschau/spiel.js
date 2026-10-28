/* Browser-Vorschau. Die C-Fassung unter spiel/ ist die Lehrreferenz. */
(function (root) {
  'use strict';
  const map = [
    '###############','#.....#.......#','#.##..#..###..#',
    '#.............#','#.##..#.......#','#.....#.......#',
    '#.....#.......#','#.....#.......#','###############'
  ];
  const guards = [{x:1,y:5,r:1},{x:9,y:5,r:2}];
  const names = {1:'Museumsschlüssel',2:'Tresorbeute',3:'Sammlermünze'};
  const same = (p,x,y) => p.x === x && p.y === y;
  const valid = (x,y) => Number.isInteger(x) && Number.isInteger(y) && x>=0 && x<15 && y>=0 && y<9;
  const floor = (x,y) => valid(x,y) && map[y][x] !== '#';
  const danger = (x,y) => guards.some(g => Math.abs(x-g.x)+Math.abs(y-g.y)<=g.r);
  function create() {
    return {x:3,y:7,turns:0,phase:0,keyActive:true,doorOpen:false,vaultLooted:false,
      coinActive:true,inventory:[],message:'Schlüssel holen, Tür öffnen, Tresor leeren und zum Ausgang zurückkehren.'};
  }
  function step(g,command) {
    if(g.phase !== 0) return -5;
    const moves={w:[0,-1],a:[-1,0],s:[0,1],d:[1,0],z:[0,0]};
    if(!Object.hasOwn(moves,command)) return -2;
    const [dx,dy]=moves[command],x=g.x+dx,y=g.y+dy;
    if(!floor(x,y)) {g.message='Dort ist eine Wand. Kein Zug verbraucht.';return 1;}
    g.message=command==='z'?'Ein Zug gewartet.':'Leise weiter.';
    if(command!=='z') {
      if(x===6 && y===3 && !g.doorOpen) {
        if(!g.inventory.includes(1)) {g.message='Die Tür ist zu. Finden Sie den Schlüssel.';return 1;}
        g.inventory=g.inventory.filter(i=>i!==1);g.doorOpen=true;
        g.message='Tür geöffnet. Der Schlüssel wurde verwendet.';
      }
      if(x===3 && y===5 && g.keyActive) {g.keyActive=false;g.inventory.unshift(1);g.message='Museumsschlüssel aufgenommen.';}
      if(x===11 && y===7 && g.coinActive) {g.coinActive=false;g.inventory.unshift(3);g.message='Sammlermünze aufgenommen. Optionaler Bonus!';}
      if(x===13 && y===1 && !g.vaultLooted) {g.vaultLooted=true;g.inventory.unshift(2);g.message='Beute gesichert! Jetzt zurück zum Ausgang.';}
      if(x===1 && y===1 && !g.inventory.includes(2)) g.message='Hier ist der Ausgang. Die Beute fehlt noch.';
    }
    g.x=x;g.y=y;g.turns++;
    if(danger(x,y)) {g.phase=2;g.message='Alarm! Sie wurden entdeckt.';}
    else if(x===1 && y===1 && g.inventory.includes(2)) {g.phase=1;g.message='Entkommen! Der Tresorraub ist gelungen.';}
    else if(g.turns>=60) {g.phase=3;g.message='Zeit abgelaufen. Die Polizei ist da.';}
    return 0;
  }
  function serialize(g) {
    if(g.phase!==0) throw new Error('Nur laufende Spiele können gespeichert werden.');
    return ['TRESORRAUB 1',`${g.x} ${g.y} ${g.turns}`,
      `${+g.keyActive} ${+g.doorOpen} ${+g.vaultLooted} ${+g.coinActive}`,
      String(g.inventory.length),...g.inventory.map(String)].join('\n')+'\n';
  }
  function deserialize(text) {
    if(text.length>2048) throw new Error('Spielstand ist zu groß.');
    const lines=text.replace(/\s+$/,'').split(/\r?\n/);
    const nums=(line,count)=>{
      const words=(line||'').trim().split(/\s+/);
      if(words.length!==count || words.some(s=>!/^[-+]?\d+$/.test(s))) throw new Error('Ungültiges Zahlenformat.');
      const values=words.map(Number);
      if(values.some(n=>!Number.isSafeInteger(n)||n < -2147483648||n>2147483647)) throw new Error('Zahlenwert zu groß.');
      return values;
    };
    if(lines[0]!=='TRESORRAUB 1') throw new Error('Unbekannte Spielstandversion.');
    const [x,y,turns]=nums(lines[1],3),flags=nums(lines[2],4),[n]=nums(lines[3],1);
    if(!floor(x,y)||danger(x,y)||turns<0||turns>=60||n<0||n>3||flags.some(v=>v!==0&&v!==1)||lines.length!==4+n)
      throw new Error('Ungültiger Spielzustand.');
    const inventory=lines.slice(4).map(line=>nums(line,1)[0]);
    if(inventory.some(i=>!Object.hasOwn(names,i))||new Set(inventory).size!==inventory.length) throw new Error('Ungültiges Inventar.');
    const [keyActive,doorOpen,vaultLooted,coinActive]=flags.map(Boolean);
    if((keyActive&&doorOpen)||inventory.includes(1)!==(!keyActive&&!doorOpen)||inventory.includes(2)!==vaultLooted||inventory.includes(3)!==!coinActive
      ||(!doorOpen&&(x>=6||vaultLooted||!coinActive))||(keyActive&&x===3&&y===5)||(coinActive&&x===11&&y===7)
      ||(!vaultLooted&&x===13&&y===1)||(vaultLooted&&x===1&&y===1)) throw new Error('Widersprüchlicher Spielstand.');
    return {x,y,turns,phase:0,keyActive,doorOpen,vaultLooted,coinActive,inventory,message:'Spielstand geladen.'};
  }
  function tile(g,x,y) {
    if(same(g,x,y)) return {text:'',icon:g.phase===2||g.phase===3?'sad':'happy',kind:'player',label:g.phase===2?'Spielfigur: entdeckt und traurig':g.phase===3?'Spielfigur: Zeit abgelaufen':'Spielfigur: lächelnd'};
    if(!floor(x,y)) return {text:'',kind:'wall',label:'Wand'};
    if(x===3&&y===5&&g.keyActive) return {text:'',icon:'key',kind:'item',label:'Schlüssel'};
    if(x===6&&y===3) return {text:'',icon:g.doorOpen?'doorOpen':'door',kind:'door',label:g.doorOpen?'Offene Tür':'Geschlossene Tür'};
    if(x===13&&y===1) return {text:'',icon:g.vaultLooted?'vaultOpen':'vault',kind:'vault',label:g.vaultLooted?'Leerer Tresor':'Tresor'};
    if(x===11&&y===7&&g.coinActive) return {text:'',icon:'coin',kind:'item',label:'Bonusmünze'};
    if(x===1&&y===1) return {text:'',icon:'exit',kind:'exit',label:'Ausgang'};
    if(guards.some(w=>w.x===x&&w.y===y)) return {text:'',icon:'angry',kind:'guard',label:'Böse schauender Wächter'};
    return danger(x,y)?{text:'',kind:'danger',label:'Überwachtes Feld'}:{text:'',kind:'floor',label:'Freies Feld'};
  }
  function icon(name) {
    const art={
      happy:'<circle cx="12" cy="12" r="10" fill="#ffd45c" stroke="#76531a"/><circle cx="8.5" cy="10" r="1.2" fill="#263247"/><circle cx="15.5" cy="10" r="1.2" fill="#263247"/><path d="M7.5 14Q12 19 16.5 14" fill="none" stroke="#523c22" stroke-width="1.8" stroke-linecap="round"/>',
      sad:'<circle cx="12" cy="12" r="10" fill="#c4d9f3" stroke="#526a8c"/><circle cx="8.5" cy="10" r="1.2" fill="#263247"/><circle cx="15.5" cy="10" r="1.2" fill="#263247"/><path d="M8 17Q12 12.5 16 17" fill="none" stroke="#354b68" stroke-width="1.8" stroke-linecap="round"/><path d="M17 12Q20 15 17 16Q14.5 15 17 12" fill="#397ac2"/>',
      angry:'<circle cx="12" cy="12" r="10" fill="#f2b5a4" stroke="#75404a"/><path d="M5 4Q12 0 19 4L18 7H6Z" fill="#3e3454"/><path d="M6.5 8L10 10M17.5 8L14 10" stroke="#382c3d" stroke-width="2" stroke-linecap="round"/><circle cx="8.5" cy="11" r="1.1" fill="#302639"/><circle cx="15.5" cy="11" r="1.1" fill="#302639"/><path d="M8 17Q12 13 16 17" fill="none" stroke="#63313b" stroke-width="1.8" stroke-linecap="round"/>',
      door:'<rect x="3" y="1.5" width="18" height="21" rx="1" fill="#755039"/><rect x="6" y="4" width="12" height="17.5" rx=".5" fill="#bf8c5d" stroke="#503824"/><rect x="8" y="6" width="8" height="6" rx=".6" fill="#d8b18a"/><circle cx="15.5" cy="15" r="1.2" fill="#f6d66c"/>',
      doorOpen:'<rect x="3" y="1.5" width="18" height="21" rx="1" fill="#755039"/><rect x="6" y="4" width="12" height="18" fill="#edf1f7"/><path d="M6 3.5L15 6.5V22L6 20Z" fill="#bf8c5d" stroke="#503824"/><circle cx="12.5" cy="14.5" r="1" fill="#f6d66c"/>',
      key:'<circle cx="8" cy="8" r="5" fill="#edc34d" stroke="#795c1a" stroke-width="1.4"/><circle cx="8" cy="8" r="1.7" fill="#eaf0f8"/><path d="M11.5 11.5L21 21M16 16L19 13M19 19L22 16" stroke="#b18420" stroke-width="3" stroke-linecap="round"/>',
      vault:'<rect x="2" y="3" width="20" height="18" rx="2" fill="#8596a8" stroke="#3d4c60"/><rect x="4.5" y="5.5" width="15" height="13" rx="1" fill="#c1ccd5" stroke="#526779"/><circle cx="12" cy="12" r="4" fill="#f2f5f7" stroke="#526779"/><path d="M12 8V16M8 12H16" stroke="#526779" stroke-width="1.4"/><path d="M4 21V22M20 21V22" stroke="#3d4c60" stroke-width="2"/>',
      vaultOpen:'<rect x="2" y="3" width="18" height="18" rx="2" fill="#8596a8" stroke="#3d4c60"/><rect x="4.5" y="5.5" width="13" height="13" fill="#36475c"/><path d="M16 4L23 6V21L16 19Z" fill="#c1ccd5" stroke="#526779"/><path d="M6 15H13" stroke="#b4c0ce"/>',
      coin:'<circle cx="12" cy="12" r="10" fill="#f1c64b" stroke="#866522" stroke-width="1.4"/><circle cx="12" cy="12" r="7.5" fill="none" stroke="#bd902b"/><path d="M12 6.5L13.7 10L17.5 10.5L14.8 13.2L15.4 17L12 15.2L8.6 17L9.2 13.2L6.5 10.5L10.3 10Z" fill="#a97719"/>',
      exit:'<rect x="2" y="2" width="15" height="20" rx="1" fill="#e8e3fc" stroke="#514379" stroke-width="1.5"/><path d="M6 5H12V19H6" fill="#746292"/><path d="M11 12H22M18 8L22 12L18 16" fill="none" stroke="#3b3164" stroke-width="2.2" stroke-linecap="round" stroke-linejoin="round"/>'
    };
    return art[name]?'<svg viewBox="0 0 24 24" aria-hidden="true" focusable="false" xmlns="http://www.w3.org/2000/svg">'+art[name]+'</svg>':'';
  }
  const api={create,step,serialize,deserialize,tile,icon,map,guards};
  root.TresorDemo=api;
  if(typeof module!=='undefined'&&module.exports) module.exports=api;
  if(typeof document==='undefined') return;
  let game=create();
  const board=document.getElementById('board'),message=document.getElementById('message');
  document.querySelectorAll('[data-icon]').forEach(el=>{el.innerHTML=icon(el.dataset.icon);});
  function render() {
    board.replaceChildren();
    for(let y=0;y<9;y++) for(let x=0;x<15;x++) {
      const t=tile(game,x,y),cell=document.createElement('div');
      cell.className='cell '+t.kind;
      if(t.icon) cell.innerHTML=icon(t.icon); else cell.textContent=t.text;
      cell.title=`${t.label} (${x}, ${y})`;
      cell.setAttribute('aria-label',cell.title);board.append(cell);
    }
    document.getElementById('turns').textContent=`${game.turns} / 60`;
    document.getElementById('loot').textContent=game.inventory.includes(2)?'gesichert':'noch im Tresor';
    document.getElementById('inventory').textContent=game.inventory.map(i=>names[i]).join(' · ')||'leer';
    message.textContent=game.message;message.dataset.phase=String(game.phase);
    document.querySelectorAll('[data-move]').forEach(b=>b.disabled=game.phase!==0);
    document.getElementById('save').disabled=game.phase!==0;
  }
  document.querySelectorAll('[data-move]').forEach(b=>b.addEventListener('click',()=>{step(game,b.dataset.move);render();}));
  document.addEventListener('keydown',event=>{
    if(event.ctrlKey||event.metaKey||event.altKey||['INPUT','TEXTAREA','SELECT'].includes(event.target.tagName)) return;
    const key=({ArrowUp:'w',ArrowLeft:'a',ArrowDown:'s',ArrowRight:'d'})[event.key]||event.key.toLowerCase();
    if(['w','a','s','d','z'].includes(key)) {event.preventDefault();step(game,key);render();}
  });
  document.getElementById('restart').addEventListener('click',()=>{game=create();render();});
  document.getElementById('save').addEventListener('click',()=>{
    const blob=new Blob([serialize(game)],{type:'text/plain'}),url=URL.createObjectURL(blob),a=document.createElement('a');
    a.href=url;a.download='spielstand.sav';a.click();setTimeout(()=>URL.revokeObjectURL(url),1000);
    game.message='Spielstand als Datei heruntergeladen. Er lässt sich auch in der C-Fassung laden.';render();
  });
  document.getElementById('load').addEventListener('change',async event=>{
    const file=event.target.files[0];if(!file) return;
    try {if(file.size>2048)throw new Error('Spielstand ist zu groß.');game=deserialize(await file.text());render();}
    catch(error) {message.textContent='Laden fehlgeschlagen: '+error.message+' Das laufende Spiel bleibt erhalten.';}
    event.target.value='';
  });
  render();
})(globalThis);
