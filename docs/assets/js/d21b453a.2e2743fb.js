"use strict";(self.webpackChunkdocs=self.webpackChunkdocs||[]).push([[330],{3905:(e,t,n)=>{n.d(t,{Zo:()=>u,kt:()=>f});var r=n(7294);function a(e,t,n){return t in e?Object.defineProperty(e,t,{value:n,enumerable:!0,configurable:!0,writable:!0}):e[t]=n,e}function o(e,t){var n=Object.keys(e);if(Object.getOwnPropertySymbols){var r=Object.getOwnPropertySymbols(e);t&&(r=r.filter((function(t){return Object.getOwnPropertyDescriptor(e,t).enumerable}))),n.push.apply(n,r)}return n}function l(e){for(var t=1;t<arguments.length;t++){var n=null!=arguments[t]?arguments[t]:{};t%2?o(Object(n),!0).forEach((function(t){a(e,t,n[t])})):Object.getOwnPropertyDescriptors?Object.defineProperties(e,Object.getOwnPropertyDescriptors(n)):o(Object(n)).forEach((function(t){Object.defineProperty(e,t,Object.getOwnPropertyDescriptor(n,t))}))}return e}function i(e,t){if(null==e)return{};var n,r,a=function(e,t){if(null==e)return{};var n,r,a={},o=Object.keys(e);for(r=0;r<o.length;r++)n=o[r],t.indexOf(n)>=0||(a[n]=e[n]);return a}(e,t);if(Object.getOwnPropertySymbols){var o=Object.getOwnPropertySymbols(e);for(r=0;r<o.length;r++)n=o[r],t.indexOf(n)>=0||Object.prototype.propertyIsEnumerable.call(e,n)&&(a[n]=e[n])}return a}var s=r.createContext({}),c=function(e){var t=r.useContext(s),n=t;return e&&(n="function"==typeof e?e(t):l(l({},t),e)),n},u=function(e){var t=c(e.components);return r.createElement(s.Provider,{value:t},e.children)},p="mdxType",m={inlineCode:"code",wrapper:function(e){var t=e.children;return r.createElement(r.Fragment,{},t)}},d=r.forwardRef((function(e,t){var n=e.components,a=e.mdxType,o=e.originalType,s=e.parentName,u=i(e,["components","mdxType","originalType","parentName"]),p=c(n),d=a,f=p["".concat(s,".").concat(d)]||p[d]||m[d]||o;return n?r.createElement(f,l(l({ref:t},u),{},{components:n})):r.createElement(f,l({ref:t},u))}));function f(e,t){var n=arguments,a=t&&t.mdxType;if("string"==typeof e||a){var o=n.length,l=new Array(o);l[0]=d;var i={};for(var s in t)hasOwnProperty.call(t,s)&&(i[s]=t[s]);i.originalType=e,i[p]="string"==typeof e?e:a,l[1]=i;for(var c=2;c<o;c++)l[c]=n[c];return r.createElement.apply(null,l)}return r.createElement.apply(null,n)}d.displayName="MDXCreateElement"},9440:(e,t,n)=>{n.r(t),n.d(t,{assets:()=>s,contentTitle:()=>l,default:()=>p,frontMatter:()=>o,metadata:()=>i,toc:()=>c});var r=n(7462),a=(n(7294),n(3905));const o={sidebar_label:"Set Spatial Environment Scale",title:"Set Spatial Environment Scale"},l=void 0,i={unversionedId:"blueprints/Functions/set-spatial-environment-scale",id:"blueprints/Functions/set-spatial-environment-scale",title:"Set Spatial Environment Scale",description:'Sets the spatial environment scale. The larger the scale, the longer the distance at which the spatial audio attenuates. To get the best experience, the scale should be set separately for each level. The default value of "1.0" means that audio will fall completely silent at a distance of 10000 units (10000 cm/100 m).',source:"@site/docs/blueprints/Functions/set-spatial-environment-scale.md",sourceDirName:"blueprints/Functions",slug:"/blueprints/Functions/set-spatial-environment-scale",permalink:"/comms-sdk-unreal/docs/blueprints/Functions/set-spatial-environment-scale",draft:!1,editUrl:"https://github.com/facebook/docusaurus/tree/main/packages/create-docusaurus/templates/shared/docs/blueprints/Functions/set-spatial-environment-scale.md",tags:[],version:"current",frontMatter:{sidebar_label:"Set Spatial Environment Scale",title:"Set Spatial Environment Scale"},sidebar:"tutorialSidebar",previous:{title:"Set Local Player Rotation",permalink:"/comms-sdk-unreal/docs/blueprints/Functions/set-local-player-rotation"},next:{title:"Set Token",permalink:"/comms-sdk-unreal/docs/blueprints/Functions/set-token"}},s={},c=[],u={toc:c};function p(e){let{components:t,...n}=e;return(0,a.kt)("wrapper",(0,r.Z)({},u,n,{components:t,mdxType:"MDXLayout"}),(0,a.kt)("p",null,'Sets the spatial environment scale. The larger the scale, the longer the distance at which the spatial audio attenuates. To get the best experience, the scale should be set separately for each level. The default value of "1.0" means that audio will fall completely silent at a distance of 10000 units (10000 cm/100 m).'))}p.isMDXComponent=!0}}]);