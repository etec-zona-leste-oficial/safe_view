# Projeto de Conclusão de Curso | Sistema IoT para diminuir os impactos de Pontos Cegos
🎯 Objetivo do Projeto
- Desenvolver um sistema IoT capaz de auxiliar motoristas na identificação dos pontos cegos do veículo, aumentando a segurança e percepção durante a condução.

# 🛠️ Tecnologias Utilizadas
Hardware

- ESP32 / ESP32-CAM
- Raspberry Pi

Software & Ferramentas

- Python
- C++
- React — 19.1.0 | React-native — 0.81.5
- JavaScript
- Firebase
- Figma
- UML

# 🤝Como contribuir
Realizamos uma pesquisa de campo e ao conversar com motoristas e ver a rotina real, percebemos ainda mais essa dificuldade. Esses relatos mostraram que o problema é constante e afeta tanto motoristas experientes quanto iniciantes, reforçando ainda mais a necessidade de uma solução prática e acessível.
Se você já passou por algo parecido, compartilhe sua experiência no nosso formulário: 🔗https://forms.gle/WmKFNv2wfDB3YCT16

# ▶️ Como Executar o Projeto

- Clone este repositório
- Instale as dependências necessárias
 # 🔧 Requisitos
- Node.js 20 (use nvm use 20.19.5)
- Java 17+
- Expo SDK 54
- npm 10+

# Ao iniciar um projeto 
- node -v (Verifique a versão do node)
- npm -v (Verifique a versão do npm)
- Vá no arquivo package.json e verifique "expo":
- o Expo Go continua recebendo pequenas atualizações internas (tipo 54.0.6, 54.0.20 etc). Porém, existe uma forma de fazer ele atualizar sozinho quando continuar dentro do mesmo SDK
- npm update expo

Se quiser garantir que tudo fique atualizado e compatível:
Atualize o Expo CLI: npm install -g expo
Dentro do projeto:
- npm update expo
- npx expo start -c

Se tudo estiver de acordo com o requisitos siga para:
  1. Instalar dependências
npm install

  2. Instalar pacotes Expo
npx expo install

  3. Rodar o app com cache limpo
npx expo start -c

# Em caso do node estar na versão mais recente 22 (o expo ainda não é bom para essa versão) mude para 20
Caso ainda não tenha crie um arquivos chamado .nvmrc (força o projeto a usar uma versão específica de Node) 
- Coloque no arquivo a versão que é para ser usada: "20"
- No prompt: "nvm use" (se der erro é porque o nvm ainda não foi instalado) 
- instale: https://github.com/coreybutler/nvm-windows/releases
- Apague e reinstale os arquivos: "node_modules", "package-lock.json"
E depois tente de novo.

# Em caso de projeto desatualizado:
Instale o SDK 54
- npx expo install expo@~54.0.0
- npx expo install react-native@latest @expo/metro-runtime@latest
- Coloque a versão atual no package.json
- Remove-Item -Recurse -Force node_modules, package-lock.json
- npm cache clean --force
- npm install
  
- Configure o ambiente (Firebase, sensores e módulos ESP32)
- Execute o projeto de acordo com os guias de cada módulo (mobile, backend e hardware)



Licenciamento e creditamento


👥 Integrantes do Grupo

* Ester Rodrigues Soares
* Gabrielly Nascimento Bento
* Gustavo Henrique Ribeiro da Silva
* Jhonata Alves do Nascimento

👥 Equipe
![Foto dos integrantes](https://github.com/user-attachments/assets/b33a6bf4-bc0c-4c46-b57b-7e503d09bbec)

