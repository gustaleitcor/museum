# 🏛️ Simulador de Museu em C++ com OpenGL

## 📖 Sobre o Projeto
Este projeto é um **simulador de museu 3D** desenvolvido em **C++** utilizando a biblioteca **OpenGL**.  O museu é construído na forma de um ciclo em que a exposição das artes se altera de acordo com o deslocamento do jogador.

---

## ⚙️ Como Compilar e Executar

Certifique-se de ter o **Meson** e o **Ninja** instalados.  

Para o setup das bibliotecas do projeto rode:

```bash
meson setup build
```

Para compilar e rodar o projeto, use:

```bash
meson compile -C build && ./build/main
```

Conceitos utilizados no desenvolvimento do projeto:  
- Conceito de oclusão foi utilizado para a renderização do céu aberto do museu, em que o *Z-BUFFER* é inicialmente desativado e depois reativado para dar a impressão de profundidade no céu;
![Céu aberto do museu](./sky.png) 
- Para a iluminação do ambiente foi utilizada uma luz direcional vinda do "céu", e três luzes do tipo spotlight que são revezadas para iluminar os quadros próximos ao jogador;
- Texturas foram aplicadas tanto no ambiente (céu, parede, chão, barreira e etc) quanto para renderizar as artes nas molduras;
- Para modelar a corda da barreira que fica a frente das artes no museu, utilizou-se uma curva de bézier.
![Quadro](./bezier_spotlight_texture.png) Exemplo de arte renderizada utilizando textura, iluminação de spotlight e curva de bézier para a barreira