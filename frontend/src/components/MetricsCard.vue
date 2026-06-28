<script setup>
defineProps({
  label: String,
  value: [String, Number],
  icon: String,
  colorClass: {
    type: String,
    default: 'text-blue-400'
  }
});
</script>

<template>
  <div class="glass p-4 rounded-xl flex flex-col justify-between overflow-hidden relative group transition-all duration-300 hover:bg-white/10 hover:-translate-y-0.5">
    <!-- Glow effect -->
    <div class="absolute -inset-4 bg-gradient-to-br from-white/5 to-transparent opacity-0 group-hover:opacity-100 transition-opacity blur-xl z-0"></div>
    
    <div class="relative z-10 flex items-center justify-between mb-2">
      <span class="text-xs font-semibold text-slate-400 uppercase tracking-wider">{{ label }}</span>
      <span v-if="icon" class="text-lg opacity-70" :class="colorClass">{{ icon }}</span>
    </div>
    
    <div class="relative z-10 flex items-baseline gap-2">
      <span class="text-2xl font-bold text-white tracking-tight">{{ value }}</span>
    </div>
  </div>
</template>
