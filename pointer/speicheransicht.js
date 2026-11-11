(() => {
  "use strict";
  const root = document.querySelector("[data-memory-explorer]");
  if (!root) return;
  const q = (part) => root.querySelector("[data-memory-" + part + "]");
  const casesMenu = q("case"), prev = q("prev"), next = q("next");
  const memory = q("region"), grid = q("grid"), links = q("links"), arrows = q("arrows");
  const make = (tag, cls, value) => {
    const el = document.createElement(tag);
    if (cls) el.className = cls;
    if (value !== undefined) el.textContent = value;
    return el;
  };
  const address = 8;
  const bytes = (n, size) => Array.from({length: size}, (_, i) => Number((BigInt(n) >> BigInt(8 * i)) & 255n));
  const hex = (byte) => byte.toString(16).toUpperCase().padStart(2, "0");
  const field = (name, start, data, value) => ({name, start, data, value});
  const number = (name, start, value) => ({name, start, fields: [field("", start, bytes(value, 4), value)]});
  const pointer = (name, start) => ({name, start, target: address, fields: [field("", start, bytes(address, 8), "Adresse " + address)]});
  const fach = Array.from({length: 16}, (_, i) => "Info".charCodeAt(i) || 0);
  const person = (name, start, alter) => ({name, start, fields: [
    field("alter", start, bytes(alter, 4), alter),
    field("groesse", start + 4, bytes(160, 4), 160),
    field("fach", start + 8, fach, '"Info"')
  ]});
  const a = number("var", address, 10), b = number("var", address, 20);
  const poi = pointer("p", 16), param = pointer("param", 24);
  const x = person("x", address, 20), xNeu = person("x", address, 21), bd = pointer("bd", 32);
  const intStart = "    int var = 10;";
  const pointerStart = "    int *p = &var;";
  const personStart = '    struct person x = {20, 160, "Info"};';
  const intPrint = '    printf("%d\\n", var);';
  const personPrint = '    printf("%d\\n", x.alter);';
  const personType = ["struct person {", "    int alter;", "    int groesse;", "    char fach[16];", "};", ""];
  const code = (def, body, types = []) => ["#include <stdio.h>", "", ...types, ...def, "", "int main(void) {", ...body, "    return 0;", "}"];
  const step = (line, memory, note, output) => ({line, memory, note, output});
  const variants = [
    {end: 16, code: code(["void verdopple(int param) {", "    param = param * 2;", "}"],
      [intStart, "    verdopple(var);", intPrint]),
      steps: [
        step(intStart, [a], "var enthält 10."),
        step("    verdopple(var);", [a, number("param", 12, 10)], "param erhält eine Kopie des Werts."),
        step("    param = param * 2;", [a, number("param", 12, 20)], "Die Kopie wird 20. var bleibt 10."),
        step(intPrint, [a], "Der Aufruf ist beendet. var ist unverändert.", "10")
      ]},
    {end: 32, code: code(["void verdopple(int *param) {", "    *param = *param * 2;", "}"],
      [intStart, pointerStart, "    verdopple(p);", intPrint]),
      steps: [
        step(intStart, [a], "var enthält 10."),
        step(pointerStart, [a, poi], "p speichert die Adresse von var."),
        step("    verdopple(p);", [a, poi, param], "param erhält eine Kopie der Adresse. Beide Pointer zeigen auf var."),
        step("    *param = *param * 2;", [b, poi, param], "Über *param wird var zu 20. Die Pointer behalten ihre Adressen."),
        step(intPrint, [b, poi], "Der Aufrufer liest den geänderten Wert.", "20")
      ]},
    {end: 56, code: code(["void geburtstag(struct person bd) {", "    bd.alter = bd.alter + 1;", "}"],
      [personStart, "    geburtstag(x);", personPrint], personType),
      steps: [
        step(personStart, [x], "x enthält alle Felder der Person."),
        step("    geburtstag(x);", [x, person("bd", 32, 20)], "bd erhält eine vollständige Kopie mit 24 Byte."),
        step("    bd.alter = bd.alter + 1;", [x, person("bd", 32, 21)], "Nur bd.alter wird 21. x.alter bleibt 20."),
        step(personPrint, [x], "Der Aufruf ist beendet. Die Kopie wird nicht mehr dargestellt.", "20")
      ]},
    {end: 40, code: code(["void geburtstag(struct person *bd) {", "    bd->alter = bd->alter + 1;", "}"],
      [personStart, "    geburtstag(&x);", personPrint], personType),
      steps: [
        step(personStart, [x], "x enthält alle Felder der Person."),
        step("    geburtstag(&x);", [x, bd], "bd speichert die Startadresse von x. Der Parameter belegt 8 statt 24 Byte."),
        step("    bd->alter = bd->alter + 1;", [xNeu, bd], "Das erste Feld des Originals ändert sich: x.alter wird 21."),
        step(personPrint, [xNeu], "Der Aufrufer liest das geänderte Feld.", "21")
      ]}
  ];
  const flatten = (objects) => {
    const m = new Map();
    for (const obj of objects) for (const f of obj.fields) f.data.forEach((byte, i) => m.set(f.start + i, byte));
    return m;
  };
  let index = 0;
  let connections = [];

  function drawArrows() {
    arrows.replaceChildren();
    const r = memory.getBoundingClientRect();
    if (!r.width || !r.height) return;
    links.setAttribute("viewBox", "0 0 " + r.width + " " + r.height);
    connections.forEach(([from, to], i) => {
      const f = from.getBoundingClientRect(), t = to.getBoundingClientRect();
      const x1 = f.left - r.left, y1 = f.top + f.height / 2 - r.top;
      const x2 = t.left - r.left - 1, y2 = t.top + t.height * (.65 - .25 * i) - r.top;
      const bend = x1 - 9 - 10 * i;
      const path = document.createElementNS("http://www.w3.org/2000/svg", "path");
      path.setAttribute("d", "M" + x1 + " " + y1 + " H" + bend + " V" + y2 + " H" + x2);
      path.setAttribute("marker-end", "url(#memory-arrow)");
      arrows.append(path);
    });
  }

  function render() {
    const variant = variants[Number(casesMenu.value)];
    const current = variant.steps[index];
    q("step").textContent = (index + 1) + " / " + variant.steps.length;
    q("note").textContent = current.note;
    prev.disabled = index === 0;
    next.disabled = index === variant.steps.length - 1;
    const output = q("output");
    output.hidden = current.output === undefined;
    output.querySelector("code").textContent = current.output || "";
    q("code").replaceChildren(...variant.code.map((line, i) => {
      const row = make("span", "memory-line");
      const no = make("small", "", String(i + 1));
      no.setAttribute("aria-hidden", "true");
      if (line === current.line) row.setAttribute("aria-current", "step");
      row.append(no, make("code", "", line || "\u00a0"));
      return row;
    }));

    const present = flatten(current.memory);
    const previous = flatten(index > 0 ? variant.steps[index - 1].memory : []);
    const content = document.createDocumentFragment(), cells = new Map();
    for (let addr = 0; addr < variant.end; addr++) {
      const byte = present.get(addr);
      const cell = make("div", "memory-byte");
      cell.dataset.byte = addr;
      cell.dataset.occupied = String(byte !== undefined);
      cell.dataset.changed = String(byte !== undefined && byte !== previous.get(addr));
      cell.style.gridColumn = (addr % 4) + 1;
      cell.style.gridRow = Math.floor(addr / 4) + 1;
      cell.append(make("small", "", addr), make("code", "", byte === undefined ? "—" : hex(byte)));
      content.append(cell);
      cells.set(addr, cell);
    }
    const descriptions = [];
    for (const obj of current.memory) {
      const size = obj.fields.reduce((n, f) => n + f.data.length, 0);
      for (let addr = obj.start; addr < obj.start + size; addr += 4) {
        const f = obj.fields.find(item => item.start === addr);
        const label = make("div", "memory-field");
        label.style.gridColumn = 5;
        label.style.gridRow = Math.floor(addr / 4) + 1;
        label.dataset.continues = String(addr + 4 < obj.start + size);
        if (addr === obj.start) {
          const typed = obj.fields.length === 1 && obj.target === undefined ? " = " + f.value : "";
          label.append(make("span", "", obj.name + typed + " · " + size + " Byte"));
          if (obj.target !== undefined) label.append(make("code", "", "→ Adresse " + obj.target));
        }
        if (f && f.name) label.append(make("code", "", f.name + " = " + f.value));
        content.append(label);
      }
      descriptions.push(obj.name + " ab Adresse " + obj.start + ", " + size + " Byte: " +
        obj.fields.map(f => (f.name ? f.name + " " : "") + f.value + " (" + f.data.map(hex).join(" ") + ")").join("; "));
    }
    grid.replaceChildren(content);
    grid.setAttribute("aria-label", "Speicherausschnitt ab Adresse 0. " + descriptions.join(". "));
    connections = current.memory.filter(o => o.target !== undefined).map(o => [cells.get(o.start), cells.get(o.target)]);
    drawArrows();
  }

  const move = (delta) => {
    const target = index + delta;
    if (target < 0 || target >= variants[Number(casesMenu.value)].steps.length) return;
    const focused = document.activeElement;
    index = target;
    render();
    if (focused === next && next.disabled) prev.focus();
    if (focused === prev && prev.disabled) next.focus();
  };
  prev.addEventListener("click", () => move(-1));
  next.addEventListener("click", () => move(1));
  casesMenu.addEventListener("change", () => { index = 0; render(); });
  root.addEventListener("keydown", (event) => {
    if (event.target !== prev && event.target !== next) return;
    if (event.key !== "ArrowLeft" && event.key !== "ArrowRight") return;
    event.preventDefault();
    move(event.key === "ArrowLeft" ? -1 : 1);
  });
  render();
  if (typeof ResizeObserver === "function") new ResizeObserver(drawArrows).observe(memory);
})();
