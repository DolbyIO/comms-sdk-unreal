"use strict";(self.webpackChunkdocs=self.webpackChunkdocs||[]).push([[939],{3905:(e,t,n)=>{n.d(t,{Zo:()=>l,kt:()=>f});var r=n(7294);function o(e,t,n){return t in e?Object.defineProperty(e,t,{value:n,enumerable:!0,configurable:!0,writable:!0}):e[t]=n,e}function i(e,t){var n=Object.keys(e);if(Object.getOwnPropertySymbols){var r=Object.getOwnPropertySymbols(e);t&&(r=r.filter((function(t){return Object.getOwnPropertyDescriptor(e,t).enumerable}))),n.push.apply(n,r)}return n}function c(e){for(var t=1;t<arguments.length;t++){var n=null!=arguments[t]?arguments[t]:{};t%2?i(Object(n),!0).forEach((function(t){o(e,t,n[t])})):Object.getOwnPropertyDescriptors?Object.defineProperties(e,Object.getOwnPropertyDescriptors(n)):i(Object(n)).forEach((function(t){Object.defineProperty(e,t,Object.getOwnPropertyDescriptor(n,t))}))}return e}function a(e,t){if(null==e)return{};var n,r,o=function(e,t){if(null==e)return{};var n,r,o={},i=Object.keys(e);for(r=0;r<i.length;r++)n=i[r],t.indexOf(n)>=0||(o[n]=e[n]);return o}(e,t);if(Object.getOwnPropertySymbols){var i=Object.getOwnPropertySymbols(e);for(r=0;r<i.length;r++)n=i[r],t.indexOf(n)>=0||Object.prototype.propertyIsEnumerable.call(e,n)&&(o[n]=e[n])}return o}var s=r.createContext({}),u=function(e){var t=r.useContext(s),n=t;return e&&(n="function"==typeof e?e(t):c(c({},t),e)),n},l=function(e){var t=u(e.components);return r.createElement(s.Provider,{value:t},e.children)},p="mdxType",d={inlineCode:"code",wrapper:function(e){var t=e.children;return r.createElement(r.Fragment,{},t)}},m=r.forwardRef((function(e,t){var n=e.components,o=e.mdxType,i=e.originalType,s=e.parentName,l=a(e,["components","mdxType","originalType","parentName"]),p=u(n),m=o,f=p["".concat(s,".").concat(m)]||p[m]||d[m]||i;return n?r.createElement(f,c(c({ref:t},l),{},{components:n})):r.createElement(f,c({ref:t},l))}));function f(e,t){var n=arguments,o=t&&t.mdxType;if("string"==typeof e||o){var i=n.length,c=new Array(i);c[0]=m;var a={};for(var s in t)hasOwnProperty.call(t,s)&&(a[s]=t[s]);a.originalType=e,a[p]="string"==typeof e?e:o,c[1]=a;for(var u=2;u<i;u++)c[u]=n[u];return r.createElement.apply(null,c)}return r.createElement.apply(null,n)}m.displayName="MDXCreateElement"},8331:(e,t,n)=>{n.r(t),n.d(t,{assets:()=>s,contentTitle:()=>c,default:()=>p,frontMatter:()=>i,metadata:()=>a,toc:()=>u});var r=n(7462),o=(n(7294),n(3905));const i={sidebar_label:"Get Token",title:"Get Token"},c=void 0,a={unversionedId:"blueprints/Functions/get-token",id:"blueprints/Functions/get-token",title:"Get Token",description:"For convenience during early development and prototyping, this function is provided to acquire the client access token directly from within the application.",source:"@site/docs/blueprints/Functions/get-token.md",sourceDirName:"blueprints/Functions",slug:"/blueprints/Functions/get-token",permalink:"/comms-sdk-unreal/docs/blueprints/Functions/get-token",draft:!1,editUrl:"https://github.com/facebook/docusaurus/tree/main/packages/create-docusaurus/templates/shared/docs/blueprints/Functions/get-token.md",tags:[],version:"current",frontMatter:{sidebar_label:"Get Token",title:"Get Token"},sidebar:"tutorialSidebar",previous:{title:"Get Texture",permalink:"/comms-sdk-unreal/docs/blueprints/Functions/get-texture"},next:{title:"Mute Input",permalink:"/comms-sdk-unreal/docs/blueprints/Functions/mute-input"}},s={},u=[],l={toc:u};function p(e){let{components:t,...i}=e;return(0,o.kt)("wrapper",(0,r.Z)({},l,i,{components:t,mdxType:"MDXLayout"}),(0,o.kt)("p",null,"For convenience during early development and prototyping, this function is provided to acquire the client access token directly from within the application. "),(0,o.kt)("blockquote",null,(0,o.kt)("p",{parentName:"blockquote"},"\u26a0\ufe0f Using this function effectively distributes the permanent app credential with your Unity application, which is not safe for production deployment. Follow our ",(0,o.kt)("a",{parentName:"p",href:"https://docs.dolby.io/communications-apis/docs/guides-client-authentication"},"security best practices")," here to set up a server through which you can acquire a temporary client access token.")),(0,o.kt)("p",null,(0,o.kt)("img",{alt:"Sample",src:n(3189).Z,width:"913",height:"278"})))}p.isMDXComponent=!0},3189:(e,t,n)=>{n.d(t,{Z:()=>r});const r=n.p+"assets/images/get_dolbyio_token-ea452c968a72392ea34f0665d7e996c3.PNG"}}]);