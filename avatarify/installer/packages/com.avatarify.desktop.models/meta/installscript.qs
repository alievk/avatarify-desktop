function Component() {
}

Component.prototype.createOperationsForArchive = function(archive) {
    // don't use the default operation
    // component.createOperationsForArchive(archive);

    // add an extract operation with a modified path
    component.addOperation("Extract", archive, "@HomeDir@/.avatarify");
}