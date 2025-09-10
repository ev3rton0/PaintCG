
![alt text](<src/logo Paint.png>)

## ❯ Sobre o Projeto  
**PaintCG** é uma aplicação de **desenho vetorial 2D** desenvolvida como projeto para a disciplina de **Computação Gráfica** do curso de **Ciência da Computação da Universidade Federal do Cariri (UFCA)**.  
A aplicação foi construída do zero em **linguagem C** utilizando a biblioteca gráfica **OpenGL**.  

O objetivo principal é permitir a **criação, seleção e manipulação de primitivas geométricas** através de uma interface gráfica interativa, implementando **transformações matriciais** e funcionalidades de **persistência de dados**.  




## ✨ Funcionalidades Implementadas  

- **Criação de Primitivas Geométricas**  
  - Pontos  
  - Segmentos de Reta  
  - Polígonos convexos e côncavos (com suporte a *tessellation*)  

- **Interface Gráfica Interativa**  
  - Menu lateral com botões para seleção de modo  
  - Feedback visual em tempo real durante a criação de objetos  

- **Seleção e Manipulação de Objetos**  
  - Seleção individual de qualquer objeto na tela  
  - Exclusão de objetos selecionados  

- **Transformações Geométricas 2D**  
  - **Translação**: arrastar e soltar objetos com o mouse  
  - **Rotação**: girar objetos em torno de seu próprio centro (ou da origem, no caso de pontos)  
  - **Escala**: aumentar e diminuir o tamanho de objetos em relação ao seu centro  
  - **Reflexão**: espelhar objetos nos eixos X e Y  
  - **Cisalhamento**: distorcer objetos nos eixos X e Y  

- **Persistência de Dados**  
  - **Salvar**: estado atual do desenho em um arquivo `.txt`  
  - **Carregar**: abrir desenho previamente salvo  




## 🎮 Como Usar  

A interação é dividida entre **menu lateral (mouse)** e **atalhos de teclado**.  

### 🖱️ Menu e Mouse  
- **Botões de Modo**: clique em `Ponto`, `Reta` ou `Polígono` para criar cada primitiva.  
- **Criar Ponto**: clique na área de desenho.  
- **Criar Reta**: clique no ponto inicial e depois no final.  
- **Criar Polígono**: clique para adicionar vértices → pressione `f` para finalizar.  
- **Botão Selecionar**: clique em um objeto para selecioná-lo (ele ficará em destaque).  
- **Arrastar Objeto**: no modo seleção, arraste com o mouse.  
- **Botão Limpar Tudo**: remove todos os objetos da tela.  

### ⌨️ Teclado (com objeto selecionado)  
- `m` → Eliminar Arestas Côncavas
- `r` → Rotacionar anti-horário  
- `t` → Rotacionar horário  
- `+` → Aumentar escala  
- `-` → Diminuir escala  
- `x` → Cisalhamento no eixo X  
- `c` → Cisalhamento no eixo Y  
- `v` → Refletir no eixo X  
- `h` → Refletir no eixo Y  
- `d` → Deselecionar  
- `DEL` → Deletar objeto  

### 💾 Arquivo  
- `s` → Salvar projeto em arquivo  
- `l` → Carregar projeto salvo  

---

## ⚙️ Requisitos e Compilação  

### Requisitos  
- Compilador **C** (MinGW no Windows, GCC no Linux)  
- **freeglut** (gerenciamento de janelas/eventos OpenGL)  
- **OpenGL** (geralmente incluso nos drivers de vídeo)  

### Tutorial (Code::Blocks)  
1. Instale o Code::Blocks com MinGW.  
2. Instale o **freeglut**:  
   - Baixe o pacote de desenvolvimento.  
   - Copie os `.h` → `include/GL` do MinGW  
   - Copie os `.a` ou `.lib` → `lib` do MinGW  
   - Copie o `.dll` → pasta do executável (`bin/Debug/`)  
3. Abra o projeto `paintCG.cbp` no Code::Blocks.  
4. Configure o Linker:  
   - Vá em **Project > Build options...**  
   - Em **Linker settings** adicione:  
     ```
     opengl32
     freeglut
     glu32
     ```  
5. Compile e execute (`F9`).  



## 👥 Autores  
- Antonio Nunes de Oliveira Filho
- Everton Lohan Pereira Ferreira
- José Vitor Dias dos Santos
- Arthur Ricardo Macêdo Pereira


