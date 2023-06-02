---
sidebar_position: 7
sidebar_label: Rendering Remote Video
title: Rendering Remote Video
---

This tutorial explains how to render camera video coming from other participants.

## Prerequisites

Before you start, follow the [common setup](common-setup) tutorial.

## Set up remote video rendering

1. Find `BP_DolbyIOSingleVideoPlane` in the plugin's content in `Content Browser` and drag it onto the scene.

> **_NOTE:_** If you do not see this item, go to the `Content Browser` settings and tick `Show Plugin Content`.

2. Rescale the plane to the desired aspect ratio, for example, [6.4, 3.6, 1.0], and rotate it by [90, 0, 90] to make it face the player start, as in the following example:

![](../../static/img/video-plane-result.png)

If you launch the game now, assuming you successfully connect to a conference with at least one remote participant video enabled, you should see one video feed on the plane.

> **_NOTE:_** This particular implementation allows only one remote video feed. However, you can also create multiple planes, assign them to specific participants, and heavily modify this sample.

## How it works

`BP_DolbyIOSingleVideoPlane` is a sample Blueprint with a simple `Event Graph`, which handles [`On Video Track Added`](../blueprints/Events/on-video-track-added) and [`On Video Track Removed`](../blueprints/Events/on-video-track-removed).

![](../../static/img/remote-video-eg.png)

It also contains a generic `Construction Script` to set up the plane's material:

![](../../static/img/video-plane-cs.png)