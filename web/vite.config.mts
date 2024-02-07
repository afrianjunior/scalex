/// <reference types="vitest" />
/// <reference types="vite/client" />
import { fileURLToPath, URL } from 'node:url'
import { defineConfig } from 'vite';
import createReactPlugin from '@vitejs/plugin-react';
import createReScriptPlugin from '@jihchi/vite-plugin-rescript';
import { viteSingleFile } from "vite-plugin-singlefile"

// https://vitejs.dev/config/
export default defineConfig({
  plugins: [createReactPlugin(), createReScriptPlugin(), viteSingleFile()],
  build: {
    target: 'es2015'
  },
  resolve: {
    alias: {
      '@': fileURLToPath(new URL('./src', import.meta.url))
    }
  },
  test: {
    include: ['tests/**/*_test*.js'],
    globals: true,
    environment: 'jsdom',
    setupFiles: './tests/setup.ts',
  },
});
