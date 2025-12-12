# Projeto de Conclusão de Curso | Sistema IoT para diminuir os impactos de Pontos Cegos
 
- Temos como objetivo geral desenvolver e implementar um dispositivo IoT constituído por câmeras e sensores, integrado a um aplicativo capaz de transmitir os dados ao motorista, permitindo o monitoramento acerca do veículo.

- Temos como alvo o âmbito do setor de transportes, marcado por demandas crescentes e condições desafiadoras, buscamos atingir empresas de pequeno porte e motoristas autônomosque priorizam sua segurança e a de terceiros.

# 🛠️ Tecnologias Utilizadas
Hardware

- Sensores Ultrassônicos
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
1. Requisitos

Antes de iniciar, certifique-se de que seu ambiente possui:

- Node.js 20 (recomendado: nvm use 20.19.5)
- npm 10+
- Java 17+
- Expo SDK 54
- NVM instalado (para gerenciar versões do Node)

Ambiente configurado para:

- Firebase
- Sensores e módulos ESP32 / ESP32-CAM
- Raspberry Pi

2. Verificações Iniciais

Antes de rodar o projeto:

- node -v
- npm -v

(Opcional) Atualizar Expo para evitar conflitos
- npm install -g expo
- npm update expo
- npx expo start -c
  
3. Executando o Projeto
- git clone <URL_DO_REPOSITORIO>
- npm install
- npx expo install
- npx expo start -c

4. Criar arquivo .nvmrc

O Expo ainda não oferece suporte total ao Node 22. Caso seu sistema atualize automaticamente, siga os passos:

Na raiz do projeto, crie um arquivo chamado .nvmrc contendo: 20
Forçar uso da versão correta
- nvm use

Reinstalar dependências
- rm -rf node_modules package-lock.json
- npm install

Se ocorrer erro, instale o NVM para Windows:
https://github.com/coreybutler/nvm-windows/releases


5. Configurações Complementares

Após concluir os passos acima, configure:

- Integração com sensores
- Comunicação com módulos ESP32 / ESP32-CAM
- Módulo Raspberry Pi
  
<!-- ADD SOBRE O IOT (É BASICAMENTE UM RESUMO DA DOCUMENTAÇÃO DOS COMPONENTES ELETRONICOS SÓ QUE AGORA PARA FAZE-LO FUNCIONAR INTEGRADO COM O APP) !-->

# 📄 Licenciamento e creditamento


👥 Integrantes do Grupo

* Ester Rodrigues Soares - https://github.com/EsterRS7
* Gabrielly Nascimento Bento - https://github.com/Gabr1ell1
* Gustavo Henrique Ribeiro da Silva - https://github.com/Guzhie
* Jhonata Alves do Nascimento - https://github.com/JHOW-JHOW

👥 Equipe SafeView

![Foto dos integrantes](https://github.com/user-attachments/assets/b33a6bf4-bc0c-4c46-b57b-7e503d09bbec)



# ⚠️ Notas Técnicas
A pasta Application_SafeView não inclui o diretório node_modules, pois ele é gerado automaticamente e possui tamanho elevado. Para utilizá-la corretamente, é necessário instalar todas as dependências seguindo as instruções fornecidas acima.




