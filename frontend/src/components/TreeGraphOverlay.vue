<script setup>
import { ref, watch, onMounted, onUnmounted } from 'vue';

const props = defineProps({
  tree: Object,
  width: { type: Number, default: 800 },
  height: { type: Number, default: 600 }
});

const canvasRef = ref(null);
let ctx = null;

// Calculate how many leaf nodes are under a node to allocate horizontal space
const countLeaves = (node) => {
  if (!node) return 0;
  if (!node.divided || !node.children) {
    node.leafCount = 1;
    return 1;
  }
  let sum = 0;
  for (let child of node.children) {
    sum += countLeaves(child);
  }
  node.leafCount = sum;
  return sum;
};

// Calculate depth of the tree
const calculateDepth = (node) => {
  if (!node) return 0;
  if (!node.divided || !node.children) return 1;
  let max = 0;
  for (let child of node.children) {
    max = Math.max(max, calculateDepth(child));
  }
  return max + 1;
};

const drawTree = () => {
  if (!canvasRef.value || !props.tree) return;
  if (!ctx) ctx = canvasRef.value.getContext('2d');
  
  const w = canvasRef.value.width;
  const h = canvasRef.value.height;
  ctx.clearRect(0, 0, w, h);
  
  const totalLeaves = countLeaves(props.tree);
  const maxDepth = calculateDepth(props.tree);
  
  // Use "screen" or "lighter" for that neon glowing blending effect
  ctx.globalCompositeOperation = 'screen';
  
  const vSpacing = (h - 100) / Math.max(1, maxDepth);
  const blockWidth = 28;
  const blockHeight = 12;
  
  const drawNode = (node, x, y, availableWidth, depth) => {
    // Draw connections to children first (so they are under the blocks)
    if (node.divided && node.children) {
      let currentX = x - availableWidth / 2;
      for (let i = 0; i < node.children.length; i++) {
        const child = node.children[i];
        const childWidth = (child.leafCount / node.leafCount) * availableWidth;
        const childX = currentX + childWidth / 2;
        const childY = y + vSpacing;
        
        // ORTHOGONAL ROUTING (Tech style)
        ctx.beginPath();
        ctx.moveTo(x, y + blockHeight / 2); // Start from bottom of parent block
        
        // Go down halfway
        const midY = y + blockHeight / 2 + (vSpacing - blockHeight) / 2;
        ctx.lineTo(x, midY);
        // Go horizontally to child's X
        ctx.lineTo(childX, midY);
        // Go down to child's top
        ctx.lineTo(childX, childY - blockHeight / 2);
        
        // Techy colors based on depth/index
        const isRed = i % 2 !== 0 && depth % 2 === 0;
        ctx.strokeStyle = isRed ? 'rgba(244, 63, 94, 0.7)' : 'rgba(16, 185, 129, 0.7)'; // Red or Emerald
        ctx.lineWidth = 1.5;
        
        // Add Glow
        ctx.shadowColor = isRed ? '#f43f5e' : '#10b981';
        ctx.shadowBlur = 8;
        
        ctx.stroke();
        
        // Reset shadow for performance on next steps
        ctx.shadowBlur = 0;
        
        drawNode(child, childX, childY, childWidth, depth + 1);
        currentX += childWidth;
      }
    }
    
    // Draw node block
    const bx = x - blockWidth / 2;
    const by = y - blockHeight / 2;
    
    // Block Background
    ctx.fillStyle = node.divided ? 'rgba(8, 47, 73, 0.9)' : (node.particles > 0 ? 'rgba(67, 20, 7, 0.9)' : 'rgba(15, 23, 42, 0.9)');
    ctx.fillRect(bx, by, blockWidth, blockHeight);
    
    // Block Border & Glow
    ctx.beginPath();
    ctx.rect(bx, by, blockWidth, blockHeight);
    
    if (node.divided) {
      ctx.strokeStyle = '#06b6d4'; // Cyan
      ctx.shadowColor = '#06b6d4';
    } else if (node.particles > 0) {
      ctx.strokeStyle = '#f43f5e'; // Red
      ctx.shadowColor = '#f43f5e';
    } else {
      ctx.strokeStyle = '#475569'; // Slate
      ctx.shadowColor = 'transparent';
    }
    
    ctx.lineWidth = 1;
    ctx.shadowBlur = node.particles > 0 || node.divided ? 10 : 0;
    ctx.stroke();
    
    // Reset shadow
    ctx.shadowBlur = 0;
    
    // Text inside block (Particles count or ID)
    if (node.particles > 0) {
      ctx.fillStyle = '#fff';
      ctx.font = '8px monospace';
      ctx.textAlign = 'center';
      ctx.textBaseline = 'middle';
      ctx.fillText(node.particles, x, y + 1);
    }
  };

  drawNode(props.tree, w / 2, 50, w - 80, 0);
};

// Resize handler
const resizeCanvas = () => {
  if (canvasRef.value) {
    const parent = canvasRef.value.parentElement;
    canvasRef.value.width = parent.clientWidth;
    canvasRef.value.height = parent.clientHeight;
    drawTree();
  }
};

onMounted(() => {
  resizeCanvas();
  window.addEventListener('resize', resizeCanvas);
});

onUnmounted(() => {
  window.removeEventListener('resize', resizeCanvas);
});

watch(() => props.tree, () => {
  drawTree();
}, { deep: false });
</script>

<template>
  <div class="absolute inset-0 w-full h-full pointer-events-none overflow-hidden z-20 flex items-center justify-center bg-slate-950/60 transition-colors duration-500">
    <div class="absolute top-4 left-4 text-xs font-mono text-cyan-500 opacity-50 flex items-center gap-2 tracking-widest">
      <span class="w-2 h-2 rounded-full bg-cyan-500 animate-pulse"></span>
      QUADTREE HIERARCHY MAP
    </div>
    <canvas ref="canvasRef" class="w-full h-full"></canvas>
  </div>
</template>
