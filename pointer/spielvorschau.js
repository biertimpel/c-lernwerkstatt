(() => {
  "use strict";
  if (typeof PointerTresorDaten === "undefined") return;
  const {cells, start, sprites} = PointerTresorDaten;
  const w = cells[0].length, h = cells.length, size = 16;
  const glyph = {
    E: "ausgang", T: "tresor_geschlossen", D: "tuer_geschlossen",
    W: "waechter", k: "schluessel", c: "muenze", "@": "spielfigur_froh"
  };
  const steps = {
    left: [-1, 0], right: [1, 0], up: [0, -1], down: [0, 1]
  };
  const keys = {ArrowLeft: "left", ArrowRight: "right", ArrowUp: "up", ArrowDown: "down"};

  for (const root of document.querySelectorAll("[data-pointer-game]")) {
    const canvas = root.querySelector("[data-game-board]");
    const status = root.querySelector("[data-game-status]");
    const ctx = canvas.getContext("2d");
    let [x, y] = start;
    let zuege = 0;
    canvas.width = w * size;
    canvas.height = h * size;

    function sprite(name, col, row) {
      if (!ctx) return;
      for (const [left, top, width, height, fill] of sprites[name]) {
        ctx.fillStyle = fill;
        ctx.fillRect(col * size + left, row * size + top, width, height);
      }
    }

    function render(note = "") {
      if (ctx) ctx.clearRect(0, 0, canvas.width, canvas.height);
      for (let row = 0; row < h; row++) {
        for (let col = 0; col < w; col++) {
          const c = cells[row][col];
          sprite(c === "#" ? "wand" : c === "!" || c === "W" ? "ueberwachung" : "boden", col, row);
          const object = glyph[col === x && row === y ? "@" : c];
          if (object) sprite(object, col, row);
        }
      }
      status.textContent = "Position (" + x + ", " + y + ") · Züge " + zuege + "/60" + (note ? " · " + note : "");
      status.dataset.x = x;
      status.dataset.y = y;
      status.dataset.zuege = zuege;
      canvas.setAttribute("aria-label", "Tresorraub. " + status.textContent);
    }

    function move(direction) {
      const [dx, dy] = steps[direction];
      const nx = x + dx, ny = y + dy;
      if (nx < 0 || nx >= w || ny < 0 || ny >= h) return render("Spielfeldrand");
      const tile = cells[ny][nx];
      if (tile === "#") return render("Wand: kein Zug");
      if (tile === "D") return render("Tür verschlossen: kein Zug");
      x = nx;
      y = ny;
      zuege++;
      render();
    }

    root.querySelectorAll("[data-move]").forEach(btn => {
      btn.addEventListener("click", () => move(btn.dataset.move));
    });
    root.querySelector("[data-game-reset]").addEventListener("click", () => {
      [x, y] = start;
      zuege = 0;
      render();
    });
    root.addEventListener("keydown", (event) => {
      const direction = keys[event.key];
      if (!direction || event.target.matches("input, select, textarea")) return;
      event.preventDefault();
      if (!event.repeat) move(direction);
    });
    render();
  }
})();
