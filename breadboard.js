import React, { useState, useRef, useLayoutEffect } from "react";

export default function BreadboardDesigner({
  columns = 30,
  holeSize = 12,
  holeGap = 18,
}) {
  const containerRef = useRef(null);
  const [connections, setConnections] = useState([]);
  const [firstHole, setFirstHole] = useState(null);
  const [hoverHole, setHoverHole] = useState(null);
  const [selectedColor, setSelectedColor] = useState("#ef4444");
  const [scale, setScale] = useState(1);
  const [log, setLog] = useState([]);
  const [offset, setOffset] = useState({ x: 0, y: 0 });

  const uid = (p = "id") => `${p}-${Math.random().toString(36).slice(2, 9)}`;
  const logMessage = (msg) => setLog((l) => [...l.slice(-19), msg]);

  useLayoutEffect(() => {
    const rect = containerRef.current?.getBoundingClientRect();
    if (rect) setOffset({ x: rect.left, y: rect.top });
  }, [scale]);

  const buildHoles = () => {
    const startX = 60;
    const startY = 60;
    const rows = [];
    const gapBetweenBlocks = 5 * holeGap;
    let y = startY;

    const makeRow = (band, numRows) => {
      for (let r = 0; r < numRows; r++) {
        const row = [];
        let x = startX;
        for (let c = 0; c < columns; c++) {
          row.push({ band, r, c, x, y });
          x += holeGap;
        }
        rows.push(row);
        y += holeGap;
      }
    };

    makeRow("power-top", 2);
    y += holeGap * 2;
    makeRow("term-top", 5);
    y += gapBetweenBlocks;
    makeRow("term-bottom", 5);
    y += holeGap * 2;
    makeRow("power-bottom", 2);

    return rows;
  };

  const holes = buildHoles();
  const holeId = (h) => `${h.band}:${h.r}:${h.c}`;

  const findHole = (id) => {
    for (const row of holes) for (const h of row) if (holeId(h) === id) return h;
    return null;
  };

  const sameGroup = (h1, h2) => {
    if (h1.band.includes("power") && h1.band === h2.band && h1.r === h2.r) return true;
    if (h1.band.includes("term") && h1.band === h2.band && h1.c === h2.c) return true;
    return false;
  };

  const handleHoleClick = (h) => {
    logMessage(`Clicked: ${holeId(h)} | band=${h.band} row=${h.r} col=${h.c} x=${h.x} y=${h.y}`);

    if (!firstHole) {
      setFirstHole(h);
      logMessage(`Selected first hole: ${holeId(h)}`);
      return;
    }

    if (holeId(firstHole) === holeId(h)) {
      setFirstHole(null);
      logMessage(`Cancelled (same hole)`);
      return;
    }

    setConnections((s) => [...s, { id: uid("conn"), a: holeId(firstHole), b: holeId(h), color: selectedColor }]);
    logMessage(`Connected ${holeId(firstHole)} → ${holeId(h)}`);
    setFirstHole(null);
  };

  const deleteWire = (id) => {
    setConnections((s) => s.filter((c) => c.id !== id));
    logMessage(`Deleted connection ${id}`);
  };

  const makePath = (a, b) => {
    const c1 = { x: a.x + (b.x - a.x) / 2, y: a.y };
    const c2 = { x: a.x + (b.x - a.x) / 2, y: b.y };
    return `M ${a.x} ${a.y} C ${c1.x} ${c1.y} ${c2.x} ${c2.y} ${b.x} ${b.y}`;
  };

  const hoveredHoleObj = holes.flat().find((h) => holeId(h) === hoverHole);

  return (
    <div className="p-4">
      <div className="flex gap-4 mb-2 items-center">
        <label className="text-sm">Wire Color</label>
        <input type="color" value={selectedColor} onChange={(e) => setSelectedColor(e.target.value)} />
        <button className="px-2 py-1 bg-red-600 text-white rounded" onClick={() => setConnections([])}>Clear</button>
        <div className="ml-auto flex gap-2 items-center">
          <label className="text-sm">Zoom</label>
          <input type="range" min={0.5} max={2} step={0.05} value={scale} onChange={(e) => setScale(Number(e.target.value))} />
        </div>
      </div>

      <div ref={containerRef} className="relative bg-slate-50 shadow rounded" style={{ height: 600, overflow: "hidden" }}>
        <svg
          width="100%"
          height="100%"
          style={{ position: "absolute", top: 0, left: 0, transform: `scale(${scale})`, transformOrigin: "0 0" }}
        >
          {/* Draw holes first */}
          {holes.flat().map((h) => {
            const sameGroupHover = hoveredHoleObj && sameGroup(hoveredHoleObj, h);
            const fillColor = sameGroupHover
              ? "#bfdbfe" // blue for preconnected pins on hover
              : hoverHole === holeId(h)
              ? "#fde68a" // yellow highlight for hovered hole itself
              : "#e2e8f0"; // default

            return (
              <circle
                key={holeId(h)}
                cx={h.x}
                cy={h.y}
                r={holeSize / 2}
                fill={fillColor}
                stroke="rgba(0,0,0,0.3)"
                onClick={() => handleHoleClick(h)}
                onMouseEnter={() => setHoverHole(holeId(h))}
                onMouseLeave={() => setHoverHole(null)}
                cursor="pointer"
              />
            );
          })}

          {/* Wires always rendered on top */}
          {connections.map((c) => {
            const a = findHole(c.a);
            const b = findHole(c.b);
            if (!a || !b) return null;
            const pa = { x: a.x, y: a.y };
            const pb = { x: b.x, y: b.y };
            return (
              <path
                key={c.id}
                d={makePath(pa, pb)}
                stroke={c.color}
                strokeWidth={3}
                fill="none"
                strokeLinecap="round"
                style={{ pointerEvents: "stroke", zIndex: 10 }}
                onClick={() => deleteWire(c.id)}
              />
            );
          })}
        </svg>
      </div>

      <p className="mt-2 text-sm text-slate-600">
        Hover over a hole to see its preconnected group (blue). Click one hole, then another to create a connection. Click a wire to delete it.
      </p>

      <div className="mt-2 bg-black text-green-300 rounded p-2 text-xs font-mono h-40 overflow-auto">
        {log.map((line, i) => (
          <div key={i}>{line}</div>
        ))}
      </div>
    </div>
  );
}
